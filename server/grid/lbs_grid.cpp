#include <iostream>
#include <stdlib.h>
#include "server/grid/lbs_grid.h"

using namespace std;

//网格初始化
int lbs_grid_init(lbs_grid_t* lbs_grid, double lon1, double lon2, double lat1, double lat2, int row_num, int col_num)
{
	int i;
	//row num of grid
	lbs_grid->row_num=row_num;
	//col num of grid
	lbs_grid->col_num=col_num;
	//cell width
	lbs_grid->cell_width = (lon2 - lon1)/col_num;
	//cell height
	lbs_grid->cell_height = (lat2 - lat1)/row_num;
	//grid lon_min
	lbs_grid->lon_min=lon1;
	//grid lat_min;
	lbs_grid->lat_min=lat1;
	//hashtable
	lbs_hashtable_init(&lbs_grid->hash_table);
	//所有的cells
	lbs_grid->cell=(lbs_cell_t*)malloc((row_num*col_num)*sizeof(lbs_cell_t));
	for(i=0;i<row_num*col_num;i++)
		lbs_queue_init(&(lbs_grid->cell[i].dummy_node.queue));
	
	return 0;
}

//网格的删除
int lbs_grid_destroy(lbs_grid_t* lbs_grid)
{
	
	return 0;
}

int lbs_grid_update(lbs_grid_t* lbs_grid, double lon, double lat, uint64_t timestamp, uint32_t id)
{
	int cell_row, cell_col, cell_id, cell_id0;
	lbs_mov_node_t* p0;
	lbs_hashnode_t* q=NULL;	
	//更新后的cell_id
	cell_row=lbs_grid_cell_row(lbs_grid,lat);
	cell_col=lbs_grid_cell_col(lbs_grid,lon);
	cell_id=lbs_grid_cell_id(lbs_grid, cell_row, cell_col);	
	//若在表中找不到
	if(lbs_hashtable_get(&lbs_grid->hash_table, id)==NULL)
	{
		//重新分配链表节点p0
		p0=(lbs_mov_node_t*)malloc(sizeof(lbs_mov_node_t));
		//赋值
		p0->lon=lon;
		p0->lat=lat;
		p0->id=id;
		p0->timestamp=timestamp;	
		//插入到cell_id的双向循环链表中
		lbs_queue_insert_head(&(lbs_grid->cell[cell_id].dummy_node.queue), &(p0->queue));	
		//更新哈希表
		lbs_hashtable_set(&lbs_grid->hash_table, id, p0, cell_id);	
		return 0;
	}
	//若在表中能找到
	else 
	{
		q = lbs_hashtable_get(&lbs_grid->hash_table, id);
		cell_id0 = q->cell_id;
		p0 = q->mov_node;
		//更新出租车位置及时间戳
		p0->lon=lon;
		p0->lat=lat;
		p0->timestamp=timestamp;

		if(cell_id==cell_id0)
			return 0;
		else
		{
			//更新哈希节点的cell id
			q->cell_id=cell_id;
			//从cell id0中删除p0
			lbs_queue_remove(&(p0->queue));
			//将p0添加至cell id
			lbs_queue_insert_head(&(lbs_grid->cell[cell_id].dummy_node.queue), &(p0->queue));
			return 0;
		}	
	}	
	return -1;
}

//计算Cell Row
int lbs_grid_cell_row(lbs_grid_t* lbs_grid, double lat)
{
	int cell_row;
	cell_row=(lat-lbs_grid->lat_min)/lbs_grid->cell_height;
	
	return cell_row;
}

//计算Cell Col
int lbs_grid_cell_col(lbs_grid_t* lbs_grid, double lon)
{
	int cell_col;
	cell_col=(lon-lbs_grid->lon_min)/lbs_grid->cell_width;
	
	return cell_col;
}

//计算Cell id
int lbs_grid_cell_id(lbs_grid_t* lbs_grid, int cell_row, int cell_col)
{
	int cell_id;
	cell_id = cell_row*lbs_grid->col_num+cell_col;
	
	return cell_id;
}

//计算row和col
void lbs_grid_cell_row_col(lbs_grid_t* lbs_grid, int cell_id, int* cell_row, int* cell_col)
{
	*cell_col = cell_id%lbs_grid->col_num;
	*cell_row = cell_id/lbs_grid->cell_height;
}

////获取Cell id里面的Cell
lbs_cell_t * lbs_grid_cell(lbs_grid_t* lbs_grid, int cell_id)
{
	lbs_cell_t *p;
	p = lbs_grid->cell+cell_id;
	
	return p;
}




