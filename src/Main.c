#if defined(__linux__)
#include "/home/codeleaded/System/Static/Library/QueryLanguage.h"
#include "/home/codeleaded/System/Static/Library/Networking.h"
#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#elif defined(_WINE)
#include "/home/codeleaded/System/Static/Library/QueryLanguage.h"
#include "/home/codeleaded/System/Static/Library/Networking.h"
#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#elif defined(_WIN32)
#include "F:/home/codeleaded/System/Static/Library/QueryLanguage.h"
#include "F:/home/codeleaded/System/Static/Library/Networking.h"
#include "F:/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#elif defined(__APPLE__)
#error "Apple not supported!"
#else
#error "Platform not supported!"
#endif


QueryLanguage ql;
unsigned int playercount;

#define SIGNAL_PLAYER_ID                           (SIGNAL_START + 1)

void QueryLanguage_Proc_Connect(Server* s,Client* c,void* data,int size){
    QueryLanguage* ql = (QueryLanguage*)((void*)s - offsetof(QueryLanguage,s));
    printf("QueryLanguage_Connect(%d)\n",c->sockfd);

    Client_Signal_Send(c,SIGNAL_PLAYER_ID,&playercount,sizeof(unsigned int));
    playercount++;
}

void Setup(AlxWindow* w){
    ql = QueryLanguage_New("./data");
    QueryLanguage_Load(&ql,"./data/players.alxdb","players");

    //QueryLanguage_InterpretScript(&ql,"./code/New.alxql");
    //QueryLanguage_InterpretScript(&ql,"./code/Print.alxql");

    QueryLanguage_StartServer(
        &ql,
        5900,
        10,
        (void(*)(void*,void*,void*,int))QueryLanguage_Proc_Connect,
        (void(*)(void*,void*,void*,int))QueryLanguage_StdProc_Disconnect,
        (void(*)(void*,void*,void*,int))QueryLanguage_StdProc_Update
    );

    TT_Iter db_it = QueryLanguage_FindDB(&ql,"players");
    if(db_it!=TT_INVALID){
        Database* db = (Database*)Vector_Get(&ql.databases,db_it);
        playercount = db->data.size;
    }
}
void Update(AlxWindow* w){
    QueryLanguage_UpdateServer(&ql);

    if(Stroke(ALX_KEY_ENTER).PRESSED){
        QueryLanguage_Reload(&ql,"./data/players.alxdb","players");
    }

	Clear(BLACK);

    TT_Iter db_it = QueryLanguage_FindDB(&ql,"players");
    if(db_it!=TT_INVALID){
        Database* db = (Database*)Vector_Get(&ql.databases,db_it);
        
        CStr_RenderAlxFontf(WINDOW_STD_ARGS,GetAlxFont(),0 * GetAlxFont()->CharSizeX,0 * GetAlxFont()->CharSizeY,WHITE,"Id:");
        CStr_RenderAlxFontf(WINDOW_STD_ARGS,GetAlxFont(),10 * GetAlxFont()->CharSizeX,0 * GetAlxFont()->CharSizeY,WHITE,"Player:");
        CStr_RenderAlxFontf(WINDOW_STD_ARGS,GetAlxFont(),20 * GetAlxFont()->CharSizeX,0 * GetAlxFont()->CharSizeY,WHITE,"Score:");

        for(int i = 0;i<db->data.size;i++){
            Vector* vec = (Vector*)Vector_Get(&db->data,i);
            
            for(int j = 0;j<vec->size;j++){
                void** ptr = (void**)Vector_Get(vec,j);
                if(!ptr || !*ptr) continue;

                DB_Info* info = (DB_Info*)Vector_Get(&db->types,j);
                DB_Type* type = Database_Type_Find(db,info->tname);
            
                CStr cstr = type->fn_cstr(*ptr);
                CStr_RenderAlxFont(WINDOW_STD_ARGS,GetAlxFont(),cstr,(j + 0) * 10 * GetAlxFont()->CharSizeX,(i + 1) * GetAlxFont()->CharSizeY,WHITE);
                CStr_Free(&cstr);
            }
        }
    }
}
void Delete(AlxWindow* w){
    QueryLanguage_Save(&ql,"./data/players.alxdb","players");
    QueryLanguage_Free(&ql);
}

int main(){
    if(Create("Mario Server with Database",1200,600,1,1,Setup,Update,Delete))
        Start();
    return 0;
}