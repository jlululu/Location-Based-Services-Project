#include "lbs_index.h"

#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "lbs_nnheap.h"
#include "lbs_distance.h"
#include "lbs_grid.h"

#define LBS_LON_MIN 116
#define LBS_LON_MAX 117
#define LBS_LAT_MIN 39
#define LBS_LAT_MAX 41

#define LBS_ROW_NUM 200
#define LBS_COL_NUM 100

static lbs_grid_t lbs_grid;

// 初始化网格索引
int lbs_grid_index_init() {
  lbs_grid_init(&lbs_grid, LBS_LON_MIN, LBS_LON_MAX, LBS_LAT_MIN, LBS_LAT_MAX, LBS_ROW_NUM, LBS_COL_NUM);
  return 0;
}

// 更新接口[出租车位置更新]
int lbs_grid_index_update(double lon,
                           double lat,
                           uint64_t timestamp,
                           uint32_t id) 
{
  lbs_grid_update(&lbs_grid, lon, lat, timestamp, id);
  return 0;
}

// 范围查询接口[查询某一范围内的所有出租车信息]
int lbs_grid_index_range_query(double lon1,
                               double lon2,
                               double lat1,
                               double lat2,
                               lbs_res_node_t* out) 
{
  lbs_mov_node_t* mov_node;
  lbs_res_node_t* new_node;
  int i, j, cell_row1, cell_row2, cell_col1, cell_col2, cell_id0;
  
  //获取cell范围
  cell_row1 = lbs_grid_cell_row(&lbs_grid, lat1);
  cell_row2 = lbs_grid_cell_row(&lbs_grid, lat2);
  cell_col1 = lbs_grid_cell_col(&lbs_grid, lon1);
  cell_col2 = lbs_grid_cell_col(&lbs_grid, lon2);

  //遍历每一个cell
  for(i = cell_row1; i <= cell_row2; i++)
  {
    for(j = cell_col1; j <= cell_col2;j++)
    {
      cell_id0 = lbs_grid_cell_id(&lbs_grid, i, j);
      mov_node = &(lbs_grid.cell[cell_id0].dummy_node);
      mov_node = (lbs_mov_node_t*)mov_node->queue.next;
      //遍历该网格所有cell，当经纬符合范围，记录
      while(mov_node != &(lbs_grid.cell[cell_id0].dummy_node))
      {
        if(mov_node->lon>=lon1&&mov_node->lon<=lon2&&mov_node->lat>=lat1&&mov_node->lat<=lat2)
        {
          new_node = (lbs_res_node_t*)malloc(sizeof(lbs_res_node_t));
          new_node->lat = mov_node->lat;
          new_node->lon = mov_node->lon;
          new_node->id = mov_node->id;
          new_node->timestamp = mov_node->timestamp;
          lbs_queue_insert_head(&(out->queue), &(new_node->queue));
        }
        //移向下一个node
        mov_node = (lbs_mov_node_t*)mov_node->queue.next;
      }
    }
  }
  return 0;
}

// NN查询接口[查询离lon,lat最近的出租车]
int lbs_grid_index_nn_query(double lon, double lat, lbs_res_node_t* out) {
  // TODO: by student

   return 0;
}
