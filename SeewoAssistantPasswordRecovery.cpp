#include <cstdio>
#include <map>
#include <ctime>
#include <string>
// https://github.com/CommanderBubble/MD5
#include "md5.cpp"
const std::string seewoSettingsPath="C:\\ProgramData\\Seewo\\SeewoCore\\SeewoCore.ini";
std::map<std::string,std::string> getSettings(std::string path){
    std::map<std::string,std::string> ret_value;
    FILE* p=fopen(path.c_str(),"r"); std::string current;
    char ch=fgetc(p); while(ch!=EOF&&!feof(p)){
        if(ch=='['){ch=fgetc(p);current.clear();while(ch!=']'){current+=ch;ch=fgetc(p);}}
        else if(ch=='\n'){ch=fgetc(p); continue;}
        else{
            std::string prefix,subfix;
            while(ch!='=')
				prefix+=ch,ch=fgetc(p);
			if(ch=='=') ch=fgetc(p);
            while(ch!='\n')
				subfix+=ch,ch=fgetc(p);
            ret_value[current+"."+prefix]=subfix;
        }
        ch=fgetc(p);
    } fclose(p); return ret_value;
}
inline bool isFileExist(std::string path){
    FILE *p=fopen(path.c_str(),"r");
    if(p!=NULL) return fclose(p),true;
    else return false;
}
inline int errorExit(std::string msg){return printf("[FATAL] %s\n",msg.c_str()),1;}
std::string calcMD5FromString(std::string input){
	static char signature[512];
	static char str[512];
	memset(str,0,sizeof(str));
	memset(signature,0,sizeof signature);
	using namespace md5;
	md5_t md5object(input.c_str(),input.size(),signature);
	md5object.get_string((void*)str);
	return (std::string)str;
}
int main(){
    std::map<std::string,std::string> seewoSettings;
    if(isFileExist(seewoSettingsPath)){
        seewoSettings=getSettings(seewoSettingsPath);
    }else return errorExit("could not read seewocore.ini");

    printf("[INFO] found the md5 %s\n",seewoSettings["ADMIN.PASSWORD"].c_str());
    printf("[INFO] preparing to crack\n");

    int try_count=0;
    char password[]="000000"; double T=clock();
//    std::cout<<calcMD5FromString("665265")<<std::endl;
    for(char a='0';a<='9';++a){
        password[0]=a;
       for(char b='0';b<='9';++b){
           password[1]=b;
            for(char c='0';c<='9';++c){
                password[2]=c;
                for(char d='0';d<='9';++d){
                    password[3]=d;
                    for(char e='0';e<='9';++e){
                        password[4]=e;
                        for(char f='0';f<='9';++f){
                            password[5]=f; password[6]='\0'; ++try_count;
//                            if(strcmp(password,"665265")==0){
//                            	printf(calcMD5FromString(password).c_str());
//                            }
                            if(seewoSettings["ADMIN.PASSWORD"].find(calcMD5FromString(password))!=std::string::npos){
                            	printf("100%%...\n"); 
                                printf("\n[INFO] Successfully found password is %s!",password); getchar();
                                return 0;
                            }
                            if(clock()-T>=0.2*CLOCKS_PER_SEC){
                            	printf("%d%%... ",try_count*100/1000000);
                            	T=clock();
                            }
                        }
                    }
                }
            }
       }
    }
    printf("[INFO] Failed to find password."); getchar();
    return 0;
}
