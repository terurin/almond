#include "pidc.h"

static inline int16_t abs16(int16_t x){
    return x>=0?(+x):(-x);
}

static inline int32_t abs32(int32_t x){
    return x>=0?(+x):(-x);
}


static inline int16_t clip16(int16_t value,int16_t high,int16_t low){
    if (low<value&&value<high){
        return value;
    }else if (value>high){
        return high;
    }else {
        return low;
    }
}

static inline int32_t clip32(int32_t value,int32_t high,int32_t low){
    if (low<value&&value<high){
        return value;
    }else if (value>high){
        return high;
    }else {
        return low;
    }
}



void pidc_init(pidc_ptr obj,q0708_t p,q0708_t i,q0708_t d){
    if (obj==NULL)return;
    obj->p=p;
    obj->i=i;
    obj->d=d;
    obj->sum=0;
    obj->last=0;
    obj->sum_clip=0;//無効化
    obj->result_clip=0;//無効化
    obj->count=0;
}

void pidc_clip(pidc_ptr obj,int16_t sum,int16_t result){
    if (obj==NULL)return;
    obj->sum_clip=sum;
    obj->result_clip=result;
}


int16_t pidc_process(pidc_ptr obj,int16_t now){
    if (obj==NULL)return 0;
    q2308_t result=0;
    bool error=false;
    //Pパラメータついて
    result += (q2308_t)now*obj->p;
    
    //Iパラメータについて
    int16_t sum = obj->sum+now;
    if (obj->sum_clip!=0){
        error|=abs16(obj->sum_clip)>sum;
        sum=clip16(sum,obj->sum_clip,-obj->sum_clip);
    }
    result+=(q2308_t)sum*obj->i; 
    obj->sum=sum;//更新
    
    //Dパラメータについて
    int16_t delta = obj->last-now;
    result += (q2308_t)delta*obj->d;
    obj->last=now;//更新
    
    //出力結果を調整する
    int32_t clip=obj->result_clip<<8;
    int16_t output;
    if (obj->result_clip!=0){
        error|=abs32(result)>clip;
        output=clip32(result,clip,-clip)>>8;
    }else{
        output=result>>8;
    }
    
    //エラー検知
    obj->count =error?obj->count+1:0;
    return output;
}
        
