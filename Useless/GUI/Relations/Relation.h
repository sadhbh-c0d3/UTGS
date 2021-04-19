#ifndef __INCLUDED_USELESS_RELATIONS_H__
#define __INCLUDED_USELESS_RELATIONS_H__

namespace Useless {

/*!  \ingroup gGUI
 *   \defgroup Relations Relations
 */

/*! \ingroup Relations
 *  Relations are some shemes. They tie-on widgets together.
 */
class USELESS_API Relation
{
public:
    Relation() {}
    virtual ~Relation() {}

private:
    Relation(const Relation&);
};

};//namespace Useless

#endif  __INCLUDED_USELESS_RELATIONS_H__
