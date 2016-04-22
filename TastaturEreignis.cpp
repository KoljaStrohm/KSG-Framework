#include "TastaturEreignis.h"

using namespace Framework;

bool Framework::_ret1TE( void *param, void *obj, TastaturEreignis te )
{
    return 1;
}

bool Framework::_nurNummernTE( void *param, void *obj, TastaturEreignis te )
{
    if( te.taste >= '0' && te.taste <= '9' )
        return 1;
    if( te.taste == T_Entf || te.taste == T_BackSpace )
        return 1;
    if( te.taste == T_Links || te.taste == T_Rechts || te.taste == T_Unten || te.taste == T_Oben )
        return 1;
    return 0;
}

bool Framework::_nurHexTE( void *param, void *obj, TastaturEreignis te )
{
    if( te.taste >= '0' && te.taste <= '9' )
        return 1;
    if( te.taste >= 'A' && te.taste <= 'F' )
        return 1;
    if( te.taste >= 'a' && te.taste <= 'f' )
        return 1;
    if( te.taste == T_Entf || te.taste == T_BackSpace )
        return 1;
    if( te.taste == T_Links || te.taste == T_Rechts || te.taste == T_Unten || te.taste == T_Oben )
        return 1;
    return 0;
}