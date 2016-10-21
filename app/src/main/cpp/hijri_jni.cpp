#include <cmath>
#include <stdlib.h>
#include <string>
#include <vector>
#include "hijri_jni.h"


//utility function
int intPart(int floatNum){
    if (float(floatNum) < -0.0000001){
     return ceil(floatNum-0.0000001);
    }
    return int(floor(floatNum +0.0000001));    
}

//christian to islamic calendar
int* chrToIsl(int y, int m, int d, int diff = 0) {
    
    int jd = 0;
    if ((y>1582)||((y==1582)&&(m>10))||((y==1582)&&(m==10)&&(d>14))) 
    {
        jd=intPart((1461*(y+4800+intPart((m-14)/12)))/4)+intPart((367*(m-2-12*(intPart((m-14)/12))))/12)-
        intPart( (3* (intPart(  (y+4900+    intPart( (m-14)/12)     )/100)    )   ) /4)+d-32075;
    }
    else
    {
        jd = 367*y-intPart((7*(y+5001+intPart((m-9)/7)))/4)+intPart((275*m)/9)+d+1729777;
    }
        
        int l=jd-1948440+10632;
        int n=intPart((l-1)/10631);
        l=l-10631*n+354+diff;
        int j=(intPart((10985-l)/5316))*(intPart((50*l)/17719))+(intPart(l/5670))*(intPart((43*l)/15238));
        l=l-(intPart((30-j)/15))*(intPart((17719*j)/50))-(intPart(j/16))*(intPart((15238*j)/43))+29;
        m=intPart((24*l)/709);
        d=l-intPart((709*m)/24);
        y=30*n+j-30;

    int * res = new int [3];
    res[0] = d;
    res[1] = m;
    res[2] = y;
    return res;
}

//islamic to christian calendar
int* islToChr(int y, int m, int d, int diff = 0) {
    
    int jd=intPart((11*y+3)/30)+354*y+30*m-intPart((m-1)/2)+d+1948440-385-diff;    
    if (jd> 2299160 )
    {
        int l=jd+68569;
        int n=intPart((4*l)/146097);
        l=l-intPart((146097*n+3)/4);
        int i=intPart((4000*(l+1))/1461001);
        l=l-intPart((1461*i)/4)+31;
        int j=intPart((80*l)/2447);
        d=l-intPart((2447*j)/80);
        l=intPart(j/11);
        m=j+2-12*l;
        y=100*(n-49)+i+l;
    }    
    else    
    {
        int j=jd+1402;
        int k=intPart((j-1)/1461);
        int l=j-1461*k;
        int n=intPart((l-1)/365)-intPart(l/1461);
        int i=l-365*n+30;
        j=intPart((80*i)/2447);
        d=i-intPart((2447*j)/80);
        i=intPart(j/11);
        m=j+2-12*i;
        y=4*k+n+i-4716;
    }

    int * res = new int [3];
    res[0] = d;
    res[1] = m;
    res[2] = y;
    return res;
}

//islamic calendar to std::string
JNIEXPORT jobjectArray JNICALL Java_com_skanderjabouzi_jnitest_MainActivity_isToString (JNIEnv * env, jobject, jint y, jint m, jint d, jint diff)
{
    const char* months[] = {"Muharram","Safar","Rabi-al Awwal","Rabi-al Thani","Jumada al-Ula","Jumada al-Thani","Rajab","Sha\'ban","Ramadhan","Shawwal","Dhul Qa\'dah","Dhul Hijjah"};
    const char*  monthsH[] = {"محرّم" ,"صفر" ,"ربيع الأول" ,"ربيع الثاني"  ,"جمادى الأولى" ,"جمادى الثاني" ,"رجب" ,"شعبان" ,"رمضان" ,"شوال" ,"ذو القعدة" ,"ذو الحجة"};
    //int * res = new int [3];
    int * res = chrToIsl(y,m,d,diff);

    jobjectArray ret;

    char buf[64]; // assumed large enough to cope with result

    sprintf(buf,"%d", res[0]);
    ret = (jobjectArray)env->NewObjectArray(4,env->FindClass("java/lang/String"),env->NewStringUTF(buf));
    env->SetObjectArrayElement(ret,1,env->NewStringUTF(months[res[1]-1]));
    env->SetObjectArrayElement(ret,2,env->NewStringUTF(monthsH[res[1]-1]));

    sprintf(buf,"%d", res[2]);

    env->SetObjectArrayElement(ret,3,env->NewStringUTF(buf));

    delete[] res;
    return ret;
}

//christian calendar to std::string
JNIEXPORT jobjectArray JNICALL Java_com_skanderjabouzi_jnitest_MainActivity_chrToString(int y, int m, int d, int diff = 0)
{
    /*const char* months[] = {"January","February","March","April","May","June","July","August","September","October","November","December"};
    //int * res = new int [3];
    int * res = islToChr(y,m,d,diff);
    std::vector<std::string> ress;
    std::ostringstream ss;
    ss << res[0];
    ress.push_back(ss.str());
    ress.push_back(months[res[1]-1]);
    ss << res[2];
    ress.push_back(ss.str());
    delete[] res;
    return ress;*/
}
