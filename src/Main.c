#if defined(__linux__)
#include "/home/codeleaded/System/Static/Library/QueryLanguage.h"
#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#elif defined(_WINE)
#include "/home/codeleaded/System/Static/Library/QueryLanguage.h"
#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#elif defined(_WIN32)
#include "F:/home/codeleaded/System/Static/Library/QueryLanguage.h"
#include "F:/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#elif defined(__APPLE__)
#error "Apple not supported!"
#else
#error "Platform not supported!"
#endif



void Setup(AlxWindow* w){
    CStr path = Files_cwd();
    CStr target = CStr_Concat(path,"/data");
    CStr_Free(&path);
    //printf("Target: %s\n",target);
    
    QueryLanguage ql = QueryLanguage_New(target);
    CStr_Free(&target);

    //QueryLanguage_Load(&ql,"./Database1.alxdb","Database1");

    //QueryLanguage_InterpretLine(&ql,"CREATE db2;");
    //QueryLanguage_Print(&ql);
    //QueryLanguage_InterpretLine(&ql," -- Hello World\nLOAD db1; /* WOw \n ok */ LOAD db2;");

    QueryLanguage_InterpretScript(&ql,"./code/Main.alxql");//argv[1]
    QueryLanguage_Free(&ql);
}

void Update(AlxWindow* w){
    if(Stroke(ALX_KEY_ENTER).PRESSED){
        
    }

	Clear(BLACK);

}

void Delete(AlxWindow* w){
    
}

int main(){
    if(Create("Game with Database",2500,1200,1,1,Setup,Update,Delete))
        Start();
    return 0;
}