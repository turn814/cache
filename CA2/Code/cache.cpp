#include "cache.h"

cache::cache()
{
	for (int i=0; i<L1_CACHE_SETS; i++)
		L1[i].valid = false; 
	for (int i=0; i<L2_CACHE_SETS; i++)
		for (int j=0; j<L2_CACHE_WAYS; j++)
			L2[i][j].valid = false; 

	this->myStat.missL1 =0;
	this->myStat.missL2 =0;
	this->myStat.accL1 =0;
	this->myStat.accL2 =0;
	this->myStat.lineCounter =0;						// debug l2
}

float* cache::readStats()
{
	float stats[4] = {(float)myStat.missL1, (float)myStat.accL1, (float)myStat.missL2, (float)myStat.accL2};
	float* s = stats;
	return s;
}

void cache::search(int tag, int index)
{
	myStat.found = NOT_FOUND;
	myStat.accL1 += 1;
	//cout << "accL1" << endl;							// debug accL1
	//cout << index << ": " << L1[index].valid << ' '; // debug valid
	if ((L1[index].tag == tag) && (L1[index].valid)) {
		myStat.found = FOUND_L1;
		return;
	}
	//cout << "missL1" << endl;							// debug missL1
	myStat.missL1 += 1;
	myStat.accL2 += 1;
	for (int n = 0; n < L2_CACHE_WAYS; n++) {
		if ((L2[index][n].tag == tag) && (L2[index][n].valid)) {
			myStat.found = FOUND_L2;
			foundBlockL2 = L2[index][n];
			return;
		}
	}
	//cout << "missl2" << endl;							// debug missL2
	myStat.missL2 += 1;

	//cout << "line: " << myStat.lineCounter << " - " << "l2 miss" << endl;			// debug l2
}

void cache::readUpdate(int tag, int index, int adr, int* mem)
{
	if (myStat.found == FOUND_L1) {
		L1[index].valid = 1;
		return;
	} else if (myStat.found == FOUND_L2) {
		for (int i = 0; i < L2_CACHE_WAYS; i++) {
			if (L2[index][i].lru_position == foundBlockL2.lru_position) {
				L2[index][i].tag = L1[index].tag;
				L2[index][i].data = L1[index].data;
				for (int n = 0; n < L2_CACHE_WAYS; n++) {
					if (L2[index][n].lru_position > foundBlockL2.lru_position) {
						L2[index][n].lru_position -= 1;
					}
				}
				L2[index][i].lru_position = 7;
				L2[index][i].valid = 1;
				break;
			}
		}
		L1[index].tag = foundBlockL2.tag;
		L1[index].data = foundBlockL2.data;
		L1[index].valid = 1;
	} else if (myStat.found == NOT_FOUND) {
		for (int i = 0; i < L2_CACHE_WAYS; i++) {
			if (L2[index][i].lru_position <= 0) {
				L2[index][i].tag = L1[index].tag;
				L2[index][i].data = L1[index].data;
				for (int n = 0; n < L2_CACHE_WAYS; n++) {
					L2[index][n].lru_position -= 1;
				}
				L2[index][i].lru_position = 7;
				//for (int k = 0; k < L2_CACHE_WAYS; k++) {						// debug l2
				//	cout << L2[index][k].lru_position << ' ';
				//}
				//cout << endl;													// debug l2
				L2[index][i].valid = 1;
				break;
			}
		}
		L1[index].tag = tag;
		L1[index].data = mem[adr];
		L1[index].valid = 1;
	}
}

void cache::write(int tag, int index, int* data, int adr, int* mem)
{
	if (myStat.found == FOUND_L1) {
		L1[index].data = *data;
		L1[index].valid = 1;
		mem[adr] = *data;
	} else if (myStat.found == FOUND_L2) {
		for (int i = 0; i < L2_CACHE_WAYS; i++) {
			if (L2[index][i].lru_position == foundBlockL2.lru_position) {
				L2[index][i].tag = L1[index].tag;
				L2[index][i].data = L1[index].data;
				for (int n = 0; n < L2_CACHE_WAYS; n++) {
					if (L2[index][n].lru_position > foundBlockL2.lru_position) {
						L2[index][n].lru_position -= 1;
					}
				}
				L2[index][i].lru_position = 7;
				L2[index][i].valid = 1;
				break;
			}
		}
		L1[index].tag = tag;
		L1[index].data = *data;
		L1[index].valid = 1;
		mem[adr] = *data;
	} else if (myStat.found == NOT_FOUND) {
		mem[adr] = *data;
	}
}

void cache::controller(bool MemR, bool MemW, int* data, int adr, int* myMem)
{
	int tag = (adr & 4294967280) >> 4;
	int index = adr & 15;
	int offset = 0;

	myStat.lineCounter += 1;								// debug l2

	if (MemR) {
		search(tag, index);
		readUpdate(tag, index, adr, myMem);
	} else if (MemW) {
		search(tag, index);
		write(tag, index, data, adr, myMem);
	}
}