#include <iostream>
#include <fstream>
#include "./Global.h"
#include "./Huffman.h"
using namespace std;
int Huffman::InitTree(void){
/*
 *initial the huffman tree
 */
    //using namespace Global;
    int i;
    for(i=0; i<CODE_TYPE_NUM; i++){
        HT[i].lchild = HT[i].rchild = HT[i].parent = -1;
        HT[i].weight = 0;
        HT[i].content = sourcetype(i);
    }
    TreeRoot = i-1;
    return Ok;
}

int Huffman::DestroyTree(void){
    TreeRoot = -1;
    TreeRoot = -1;
    return Ok;
}

int Huffman::FlagFull(int flag[]){
/*
 *  return the number of the node which didn't have parents
 */
    int i,count;
    for(i=0,count=0; i<=TreeRoot; i++){
	if(flag[i]==0)
	    count += 1;
    }
    return count;
}

void Huffman::CCreateTree(void){
/*
 *  This function will be called while the user want to created a compressed file.
 */
    using namespace Huffman;
    int flag[TREE_NODE_NUM];
    for(int i=0; i<TREE_NODE_NUM; i++)
	flag[i]=0;

    int min1,min2,i;
    double w1,w2;
    while(FlagFull(flag)>1){
	for(w1=w2=MAX_DOUBLE,i=0; i<=TreeRoot; i++){
	    if(flag[i]==0){
                if(HT[i].weight<w1){
		    w1 = HT[i].weight;
		    min1 = i;
		}//if weight<w1
		else if(HT[i].weight<w2){
		    w2 = HT[i].weight;
		    min2 = i;
		}//if w1<=weight<w2
	    }//if flag==0
	}//for
	TreeRoot += 1;
	HT[TreeRoot].weight = w1+w2;
	HT[TreeRoot].lchild = min1;
	HT[TreeRoot].rchild = min2;
	HT[min1].parent = HT[min2].parent = TreeRoot;
	flag[min1] = flag[min2] = 1;
    }//while

    return;
}

void Huffman::DCreateTree(void){
/*
 *  It means that at the beginning of the compressed file,
 *  there are 8*TREE_NODE_NUMBER*sizeof(HuffmanNode) bits which
 *  are used to store the Huffman tree
 *
 */
    using namespace Global;
    if(!CodedFile.is_open())
	return;
    CodedFile.seekg(ios::beg);//moving the pointer to the head of the file:CodedFile
    CodedFile.read(TreeRoot,sizeof(TreeRoot));
    CodedFile.read(HT,TREE_NODE_NUM*sizeof(HuffmanNode));
    return;
}

int Huffman::HCode(int *code,sourcetype source){
    using namespace Global;
    int i,j;
    int path[TREE_NODE_NUM],code_loc;
    for(code_loc=0; code_loc<CODE_TYPE_NUM; code_loc++)//find the location of the node
        if(HT[code_loc].content==source)
            break;
    path[0] = code_loc;
    i=0;
    while(HT[path[i]].parent!= -1){
        path[i+1] = HT[path[i]].parent;
        i += 1;
    }
    if(path[i]!=TreeRoot)
        return Calcu_Error;
    for(j=0; i>0; i--,j++){
        if(path[i-1]==HT[path[i]].lchild)
            code[j] = 0;
        else if(path[i-1]==HT[path[i]].rchild)
            code[j] = 1;
    }
    code[j] = -1;
    return Ok;
}

//int Huffman::HDecode(Global::buffertype buffer,Global::sourcetype &S){
/*
 *   This function is used to decode the compressed file.
 *   Arguments:
 *   	buffer--coded bits
 *   	S--used to return the source value
 *   Return:
 *   	return the number of the bits that is still undecoded in the buffer because the function only decode one password every time it is called.
 */
}
