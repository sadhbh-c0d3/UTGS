#ifndef __INCLUDED_USELESS_STACK_ALLOC_H__
#define __INCLUDED_USELESS_STACK_ALLOC_H__

namespace Useless {

/*! Simple macro to help creating object on stack
 */
#define NEWONSTACK(TYPE, p) \
    char p##_chars[ sizeof(TYPE) ]; \
    {   \
    TYPE *pobj = new TYPE; \
    memcpy( p##_chars, pobj, sizeof(TYPE) ); \
    free(pobj); \
    } \
    TYPE *p = (TYPE *)p##_chars;

/*! Simple macro to help creating object on stack
 */
#define MOVEONSTACK(TYPE, p, pobj) \
    char p##_chars[ sizeof(TYPE) ]; \
    memcpy( p##_chars, pobj, sizeof(TYPE) ); \
    free(pobj); \
    TYPE *p = (TYPE *)p##_chars;

/*! Simple macro to help creating object on user memory
 */
#define MOVEINMEMORY(TYPE, p, pobj, pdest) \
    memcpy( pdest, pobj, sizeof(TYPE) ); \
    free(pobj); \
    TYPE *p = (TYPE *)pdest;

/*! Simple macro to help removing object from stack
 */
#define DELETEFROMSTACK(TYPE,p) p->~TYPE();

};//namespace Useless

#endif//__INCLUDED_USELESS_STACK_ALLOC_H__
