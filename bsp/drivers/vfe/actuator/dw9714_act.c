/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright(c) 2020 - 2023 Allwinner Technology Co.,Ltd. All rights reserved. */
/*
 * sunxi actuator driver
 */

#include "actuator.h"

#define SUNXI_ACT_NAME "dw9714_act"
#define SUNXI_ACT_ID 0x18

#define TOTAL_STEPS_ACTIVE      64

#define DLC

#define ACT_DEV_DBG_EN 0

/* print when error happens */
#define act_dev_err(x, arg...) pr_err("[ACT_ERR][dw9714_act]"x, ##arg)

/* print unconditional, for important info */
#if ACT_DEV_DBG_EN == 1
#define act_dev_dbg(x, arg...) pr_debug("[ACT][dw9714_act]"x, ##arg)
#else
#define act_dev_dbg(x, arg...)
#endif

DEFINE_MUTEX(act_mutex);
/* static struct mutex act_mutex; */
/* declaration */
static struct actuator_ctrl_t act_t;
/* static struct v4l2_subdev act_subdev; */

/* static unsigned short subdev_step_pos_table[2*TOTAL_STEPS_ACTIVE]; */

/*
 *Please implement this for each actuator device!!!
 */
static int subdev_i2c_write(struct actuator_ctrl_t *act_ctrl,
		unsigned short halfword, void *pt)
{
	int ret = 0;
	/* unsigned char waddr,bdata; */
	/* waddr = (unsigned char)(halfword>>8); */
	/* bdata = (unsigned char)(0xff&halfword); */
	/* ret = cci_write_a8_d8(&act_ctrl->sdev,waddr,bdata); */
	ret = cci_write_a0_d16(&act_ctrl->sdev, halfword);
	if (ret < 0)
		act_dev_err("subdev_i2c_write write 0x%x err!\n ", halfword);
	/* act_dev_dbg("set client->addr = [0x%x][0x%x, 0x%x]\n", halfword, waddr, bdata); */
	act_dev_dbg("set value = [0x%x]\n", halfword);
	return ret;
}

static int subdev_set_code(struct actuator_ctrl_t *act_ctrl,
		unsigned short new_code,
		unsigned short sr)
{
	int ret = 0;
	unsigned short last_code;
#ifndef DLC
	unsigned short halfword;
	unsigned short target_code;
	unsigned short delta_code;
#endif
	unsigned short dir;
	unsigned short diff;
	unsigned short range;
	/* act_dev_dbg("subdev_set_code[%d], sr[%d]\n",new_code,sr); */

	if (act_ctrl->work_status == ACT_STA_BUSY
			|| act_ctrl->work_status == ACT_STA_ERR)
		return -EBUSY;
	range = act_ctrl->active_max-act_ctrl->active_min;
	/* if(new_code>act_ctrl->active_max) */
	/* new_code=act_ctrl->active_max; */
	/* if(new_code<act_ctrl->active_min) */
	/* new_code=act_ctrl->active_min; */

	if (sr > 0xf)
		pr_info("warning!!! sr is more than 0xf\n");

	last_code = act_ctrl->curr_code;

	if (last_code > new_code) {
		diff = last_code-new_code;
		dir = 0;
	} else if (last_code < new_code) {
		diff = new_code-last_code;
		dir = 1;
	} else {
		return 0;
	}
	act_dev_dbg("act step from[%d] to [%d] range[%d] sr[%d]\n", last_code, new_code, range, sr);

	act_ctrl->work_status = ACT_STA_BUSY;

#ifdef DLC
	ret = subdev_i2c_write(act_ctrl, new_code<<4|0x3, NULL);
	goto set_code_ret;
#else
	target_code = last_code;
	if (dir == 0) {
		if ((diff >= 1) && (new_code <= (act_ctrl->active_min+range/8)) &&
				(last_code >= (act_ctrl->active_min+range/6))) {
			int delay;

			if (target_code >= act_ctrl->active_max-range/5)
				delay = 30;
			else if (target_code >= (act_ctrl->active_min+range/6))
				delay = 20;
			else
				delay = 10;

			sr = 0xf;
			target_code = range/8+act_ctrl->active_min;
			act_dev_dbg("act NEG min step to [%d]\n", target_code);
			halfword = (target_code&0x3ff)<<4 | (sr&0xf);
			ret = subdev_i2c_write(act_ctrl, halfword, NULL);
			if (ret != 0) {
				act_dev_dbg("act step target error\n");
				goto set_code_ret;
			} else
				act_ctrl->curr_code = target_code;

			usleep_range(delay*1000, delay*1100);
		} else if (diff > (range/6) && new_code < act_ctrl->active_min+(range/6)) {
			int loop = 0;

			sr = 0x3;
			if (diff > (range/3))
				delta_code = (diff+1)/3;
			else
				delta_code = (diff+1)/2;
			while (target_code > new_code) {
				loop++;
				if (loop >= 3) {
					pr_info("loop=3\n");
					break;
				}
				if (target_code > delta_code)
					target_code = (target_code-delta_code);
				else
					break;

				if (target_code <= new_code || target_code <= delta_code)
					break;

				act_dev_dbg("act NEG step to [%d]\n", target_code);
				halfword = (target_code&0x3ff)<<4 | (sr&0xf);
				ret = subdev_i2c_write(act_ctrl, halfword, NULL);
				if (ret != 0) {
					act_dev_dbg("act NEG step target error\n");
					goto set_code_ret;
				} else
					act_ctrl->curr_code = target_code;

				usleep_range(10000, 12000);
			}
		}
		/* last step */
		{
			sr = 0x3;
			act_dev_dbg("act NEG target to [%d]\n", new_code);
			halfword = (new_code&0x3ff)<<4 | (sr&0xf);
			ret = subdev_i2c_write(act_ctrl, halfword, NULL);
		}
	} else {/* POS dir */
		if ((diff > range/5) &&
				(new_code >= (act_ctrl->active_max-(range/10))) &&
				(last_code <= (act_ctrl->active_min-(range/6)))) {
			int delay;

			if (new_code >= (act_ctrl->active_max-range/10))
				delay = 10;
			else
				delay = 20;

			sr = 0xf;
			if (new_code > (act_ctrl->active_max-range/10))
				target_code = act_ctrl->active_max-range/10;
			else
				target_code = new_code;
			act_dev_dbg("act POS max step to [%d]\n", target_code);
			halfword = (target_code&0x3ff)<<4 | (sr&0xf);
			ret = subdev_i2c_write(act_ctrl, halfword, NULL);
			if (ret != 0) {
				act_dev_dbg("act POS step target error\n");
				goto set_code_ret;
			} else
				act_ctrl->curr_code = target_code;

			usleep_range(delay*1000, delay*1100);
		}
		if (target_code < new_code) {
			sr = 0x1;
			act_dev_dbg("act POS target to [%d]\n", new_code);
			halfword = (new_code&0x3ff)<<4 | (sr&0xf);
			ret = subdev_i2c_write(act_ctrl, halfword, NULL);
		}
	}
#endif

set_code_ret:
	act_ctrl->work_status = ACT_STA_HALT;
	if (ret != 0)
		act_dev_err("subdev set code err!\n");
	else
		act_ctrl->curr_code = new_code;

	return ret;
}

static int subdev_init_table(struct actuator_ctrl_t *act_ctrl,
		unsigned short ext_tbl_en,
		unsigned short ext_tbl_steps,
		unsigned short *ext_tbl)
{
	int ret = 0;
	unsigned int i;
	unsigned short *table;
	unsigned short table_size;

	act_dev_dbg("subdev_init_table\n");

	if (ext_tbl_en == 0) {
		/* table = subdev_step_pos_table; */
		/* printk("sizeof(subdev_step_pos_table)=%d\n",sizeof(subdev_step_pos_table)); */
		table_size = 2*TOTAL_STEPS_ACTIVE*sizeof(unsigned short);
		table = (unsigned short *)kmalloc(table_size, GFP_KERNEL);

		for (i = 0; i < TOTAL_STEPS_ACTIVE; i += 1) {
			table[2*i] = table[2*i+1] = (unsigned short)(act_ctrl->active_min+
					(unsigned int)(act_ctrl->active_max-act_ctrl->active_min)*i/(TOTAL_STEPS_ACTIVE-1));

		}
		act_ctrl->total_steps = TOTAL_STEPS_ACTIVE;
	} else if (ext_tbl_en == 1) {
		table = (unsigned short *)kmalloc(2*ext_tbl_steps*sizeof(unsigned short), GFP_KERNEL);
		for (i = 0; i < ext_tbl_steps; i += 1) {
			table[2*i] = ext_tbl[2*i];
			table[2*i+1] = ext_tbl[2*i+1];
		}
		act_ctrl->total_steps = ext_tbl_steps;
	} else
		return 0;

	act_ctrl->step_position_table = table;
	/* for(i=0;i<TOTAL_STEPS_ACTIVE;i+=1) */
	/* { */
	/* act_dev_dbg("TBL[%d]=%d [%d]=%d ",i+0, table[2*i+0], i+1, table[2*i+1]); */
	/* } */

	return ret;
}

static int subdev_move_pos(struct actuator_ctrl_t *act_ctrl,
		unsigned short num_steps,
		unsigned short dir)
{
	int ret = 0;
	char sign_dir = 0;
	/* unsigned short index = 0; */
	unsigned short target_pos = 0;
	/* unsigned short target_code = 0; */
	short dest_pos = 0;
	unsigned short curr_code = 0;

	act_dev_dbg("%s called, dir %d, num_steps %d\n",
			__func__,
			dir,
			num_steps);

	/* Determine sign direction */
	if (dir == MOVE_NEAR)
		sign_dir = 1;
	else if (dir == MOVE_FAR)
		sign_dir = -1;
	else {
		act_dev_err("Illegal focus direction\n");
		ret = -EINVAL;
		return ret;
	}

	/* Determine destination step position */
	dest_pos = act_ctrl->curr_pos +
		(sign_dir * num_steps);

	if (dest_pos < 0)
		dest_pos = 0;
	else if (dest_pos > act_ctrl->total_steps-1)
		dest_pos = act_ctrl->total_steps-1;

	if (dest_pos == act_ctrl->curr_pos)
		return ret;


	act_ctrl->work_status = ACT_STA_BUSY;

	curr_code = act_ctrl->step_position_table[dir+2*act_ctrl->curr_pos];

	act_dev_dbg("curr_pos =%d dest_pos =%d curr_code=%d\n",
			act_ctrl->curr_pos, dest_pos, curr_code);

	target_pos =  act_ctrl->curr_pos;
	while (target_pos != dest_pos) {
		target_pos++;
		ret = subdev_set_code(act_ctrl, act_ctrl->step_position_table[dir+2*target_pos], 0);
		if (ret == 0) {
			usleep_range(1000, 1500);
			act_ctrl->curr_pos = target_pos;
		} else
			break;
	}

	act_ctrl->work_status = ACT_STA_IDLE;

	return ret;
}

static int subdev_set_pos(struct actuator_ctrl_t *act_ctrl,
		unsigned short pos)
{
	int ret = 0;
	unsigned short target_pos = 0;

	/* Determine destination step position */
	if (pos > act_ctrl->total_steps - 1)
		target_pos = act_ctrl->total_steps - 1;
	else
		target_pos = pos;

	/* act_dev_dbg("subdev_set_pos[%d]\n",target_pos); */

	if (target_pos == act_ctrl->curr_pos)
		return ret;

	ret = subdev_set_code(act_ctrl, act_ctrl->step_position_table[2*target_pos], 0);
	if (ret == 0) {
		usleep_range(1000, 1500);
		act_ctrl->curr_pos = target_pos;
	} else
		act_dev_err("act set pos err!");

	return ret;
}

static int subdev_init(struct actuator_ctrl_t *act_ctrl,
		struct actuator_para_t *a_para)
{
	int ret = 0;
	struct actuator_para_t *para = a_para;

	if (para == NULL) {
		act_dev_err("subdev_init para error\n");
		ret = -1;
		goto subdev_init_end;
	}

	act_dev_dbg("act subdev_init\n");
	mutex_init(act_ctrl->actuator_mutex);
	/* struct v4l2_subdev *sdev = &act_ctrl->sdev; */

	act_ctrl->active_min = para->active_min;
	act_ctrl->active_max = para->active_max;

	/* init_table */
	subdev_init_table(act_ctrl, para->ext_tbl_en, para->ext_tbl_steps, para->ext_tbl);

	act_ctrl->curr_pos = 0;
	act_ctrl->curr_code = 0;

	/* ret=subdev_i2c_write(act_ctrl,0x0001, NULL);//exit power down mode */
	/* msleep(50); */
	/* ret=subdev_i2c_write(act_ctrl,0x0000, NULL);//exit power down mode */
	ret |= subdev_i2c_write(act_ctrl, 0xeca3, NULL);
	ret |= subdev_i2c_write(act_ctrl, 0xa10c, NULL);
	ret |= subdev_i2c_write(act_ctrl, 0xf2f0, NULL);
	ret |= subdev_i2c_write(act_ctrl, 0xdc51, NULL);

	ret = subdev_set_code(act_ctrl, act_ctrl->active_min, 0);

	/* ret=subdev_i2c_write(act_ctrl,0x0000, NULL);//exit power down mode */
	/* ret=subdev_i2c_write(act_ctrl,0x0138, NULL);//switching mode */
	/* ret=subdev_i2c_write(act_ctrl,0x0139, NULL);//linear mode */

	if (ret == 0)
		act_ctrl->work_status = ACT_STA_IDLE;
	else
		act_ctrl->work_status = ACT_STA_ERR;

subdev_init_end:
	return ret;
}

static int subdev_pwdn(struct actuator_ctrl_t *act_ctrl,
		unsigned short mode)
{
	int ret = 0;
	/* struct v4l2_subdev *sdev = &act_ctrl->sdev; */
	if (mode == 1) {
		act_dev_dbg("act subdev_pwdn %d\n", mode);
		act_ctrl->work_status = ACT_STA_HALT;
		/* ret=subdev_i2c_write(act_ctrl,0x0001, NULL); */
		ret = subdev_set_code(act_ctrl, act_ctrl->active_min, 0);
		usleep_range(10000, 12000);
		ret = subdev_i2c_write(act_ctrl, 0x8000, 0);
		/* if(ret==0) */
		act_ctrl->work_status = ACT_STA_SOFT_PWDN;
	} else {
		act_dev_dbg("act subdev_pwdn %d\n", mode);
		/* if gpio control */
		/* if(act_ctrl->af_io!=NULL) */
		/* setgpio */
		ret = 0;
		act_ctrl->work_status = ACT_STA_HW_PWDN;
	}

	return ret;
}

static int subdev_release(struct actuator_ctrl_t *act_ctrl,
		struct actuator_ctrl_word_t *ctrlwd)
{
	int ret = 0;

	act_dev_dbg("act subdev_release[%d] to [%d], sr[%d]\n", act_ctrl->curr_code, ctrlwd->code, ctrlwd->sr);

	if (act_ctrl->work_status != ACT_STA_HALT)
		return 0;

	ret = subdev_set_code(act_ctrl, act_ctrl->active_min, ctrlwd->sr);
	if (ret == 0) {
		act_dev_dbg("release ok!\n");
		act_ctrl->work_status = ACT_STA_IDLE;
	}
	act_dev_dbg("release finished\n");
	return ret;
}

static const struct i2c_device_id act_i2c_id[] = {
	{SUNXI_ACT_NAME, (kernel_ulong_t)&act_t},
	{ },
};

static const struct v4l2_subdev_core_ops sunxi_act_subdev_core_ops = {
	.ioctl = sunxi_actuator_ioctl,/* extracted in actuator.c */
};

static struct v4l2_subdev_ops act_subdev_ops = {
	.core = &sunxi_act_subdev_core_ops,
	/* no other ops */
};

static struct cci_driver cci_act_drv = {
	.name = SUNXI_ACT_NAME,
};

static int act_i2c_probe(struct i2c_client *client,
		const struct i2c_device_id *id)
{
	cci_dev_probe_helper(&act_t.sdev, client, &act_subdev_ops, &cci_act_drv);
	if (client)
		act_t.i2c_client = client;
	act_t.work_status = ACT_STA_HW_PWDN;
	/* add act other init para */
	act_dev_dbg("%s probe\n", SUNXI_ACT_NAME);
	return 0;
}

static int act_i2c_remove(struct i2c_client *client)
{
	cci_dev_remove_helper(client, &cci_act_drv);
	return 0;
}

static struct i2c_driver act_i2c_driver = {
	.driver = {
		.owner = THIS_MODULE,
		.name = SUNXI_ACT_NAME,
	},
	.id_table = act_i2c_id,
	.probe  = act_i2c_probe,/* actuator_i2c_probe,//, */
	.remove = act_i2c_remove,/* actuator_i2c_remove,//, */
};

static struct actuator_ctrl_t act_t = {
	.i2c_driver = &act_i2c_driver,
	/* .i2c_addr = SUNXI_ACT_ID, */
	/* .sdev */
	.sdev_ops = &act_subdev_ops,

	.set_info = {
		.total_steps = TOTAL_STEPS_ACTIVE,
	},

	.work_status = ACT_STA_HW_PWDN,
	.active_min = 100,/* ACT_DEV_MIN_CODE, */
	.active_max = 600,/* ACT_DEV_MAX_CODE, */
	.curr_pos = 0,
	.curr_code = 0,

	.actuator_mutex = &act_mutex,

	.func_tbl = {
		/* .actuator_ioctl = subdev_ioctl, */

		/* specific function */
		.actuator_init = subdev_init,
		.actuator_pwdn = subdev_pwdn,
		.actuator_i2c_write = subdev_i2c_write,
		.actuator_release = subdev_release,
		.actuator_set_code = subdev_set_code,

		.actuator_init_table = subdev_init_table,
		.actuator_move_pos = subdev_move_pos,
		.actuator_set_pos = subdev_set_pos,
	},

	.get_info = {/* just for example */
		.focal_length_num = 42,
		.focal_length_den = 10,
		.f_number_num = 265,
		.f_number_den = 100,
		.f_pix_num = 14,
		.f_pix_den = 10,
		.total_f_dist_num = 197681,
		.total_f_dist_den = 1000,
	},
};

static int __init act_mod_init(void)
{
	return cci_dev_init_helper(&act_i2c_driver);
}

static void __exit act_mod_exit(void)
{
	/* act_dev_dbg("act_mod_exit[%s]\n",act_i2c_driver.driver.name); */
	cci_dev_exit_helper(&act_i2c_driver);
}

module_init(act_mod_init);
module_exit(act_mod_exit);

MODULE_DESCRIPTION("dw9714 vcm actuator");
MODULE_LICENSE("GPL v2");
