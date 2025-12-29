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


QueryLanguage ql;

void Setup(AlxWindow* w){
    ql = QueryLanguage_New("./data");
    QueryLanguage_Load(&ql,"./data/players.alxdb","players");

    //QueryLanguage_InterpretScript(&ql,"./code/New.alxql");
    //QueryLanguage_InterpretScript(&ql,"./code/Print.alxql");
}

void Update(AlxWindow* w){


	Clear(BLACK);

    TT_Iter db_it = QueryLanguage_FindDB(&ql,"players");
    if(db_it!=TT_INVALID){
        Database* db = (Database*)Vector_Get(&ql.databases,db_it);
        
        for(int i = 0;i<db->data.size;i++){
            Vector* vec = (Vector*)Vector_Get(&db->data,i);
            
            CStr_RenderAlxFontf(WINDOW_STD_ARGS,GetAlxFont(),1 * GetAlxFont()->CharSizeX,i * GetAlxFont()->CharSizeY,WHITE,"Player:");
            for(int j = 0;j<vec->size;j++){
                void** ptr = (void**)Vector_Get(vec,j);

                Token tok = QueryLanguage_TokenDatabaseFunc(*ptr,(DB_Info*)Vector_Get(&db->types,j));
                CStr_RenderAlxFont(WINDOW_STD_ARGS,GetAlxFont(),tok.str,(j + 1) * 10 * GetAlxFont()->CharSizeX,i * GetAlxFont()->CharSizeY,WHITE);
                Token_Free(&tok);
            }
        }
    }
}

void Delete(AlxWindow* w){
    QueryLanguage_Free(&ql);
}

int main(){
    if(Create("Mario Server with Database",2500,1200,1,1,Setup,Update,Delete))
        Start();
    return 0;
}