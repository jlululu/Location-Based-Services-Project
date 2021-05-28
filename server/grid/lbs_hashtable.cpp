#include <pthread.h>
#include <stdlib.h>
#include "server/grid/lbs_defs.h"
#include "server/grid/lbs_hashtable.h"

#define RONGLIANG 20000

int lbs_hashtable_init(lbs_hashtable_t* lbs_hashtable)
{
	int i=0;
	lbs_hashtable->size = 0;
	lbs_hashtable->capacity = RONGLIANG;
	
	lbs_hashtable->hash_nodes = (lbs_hashnode_t*)malloc(lbs_hashtable->capacity*sizeof(lbs_hashnode_t));
	
	for(;i<lbs_hashtable->capacity;i++)
		lbs_queue_init(&(lbs_hashtable->hash_nodes[i].queue));
	return 0;
}

int lbs_hashtable_destroy(lbs_hashtable_t* lbs_hash_table)
{
	
	return 0;
}

int lbs_hashtable_set(lbs_hashtable_t* lbs_hash_table, uint32_t id, lbs_mov_node_t* lbs_mov_node, int cell_id)
{
	int key = id%lbs_hash_table->capacity;
	lbs_hashnode_t* p = (lbs_hashnode_t*)malloc(sizeof(lbs_hashnode_t));
	
	p->mov_node = lbs_mov_node;
	p->cell_id = cell_id;
	lbs_queue_insert_head(&(lbs_hash_table->hash_nodes[key].queue), &(p->queue)) ;
	lbs_hash_table->size++;
	
	return 0;
	
}

lbs_hashnode_t* lbs_hashtable_get(lbs_hashtable_t* lbs_hash_table, uint32_t id)
{
	lbs_hashnode_t *p, *q;
	int key = id%lbs_hash_table->capacity;
	
	p = q = &(lbs_hash_table->hash_nodes[key]);
	
	p = (lbs_hashnode_t*)p->queue.next;
	
	while(p != q)
	{
		if (p->mov_node->id == id) return p;
		p = (lbs_hashnode_t*)p->queue.next;	
	}
	return NULL;
}
