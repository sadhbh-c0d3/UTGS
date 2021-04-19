#ifndef __INCLUDED__USELESS_SAMPLE_FILE_H__
#define __INCLUDED__USELESS_SAMPLE_FILE_H__

#include <string>
#include "Useless/Sound/Sample.h"
#include "Useless/Util/SPointer.h"
#include "Useless/Sound/FileIO/SoundInterface.h"

namespace Useless {

/*! SampleFile is a Sample built from sound file
 *  Use it for sound effects
*/
class USELESS_API SampleFile : public Sample
{
public:
    SampleFile() {}
    SampleFile( const std::string &file_name );
    virtual ~SampleFile();

    virtual void GetProperties( Snd::Properties& ) const;

protected:
    virtual void Load();

    std::string                 _file_name;
    SPointer< Snd::Properties > _properties;
    SPointer< SoundLoader >     _loader;
};

};//namespace Useless
#endif//__INCLUDED__USELESS_SAMPLE_FILE_H__
