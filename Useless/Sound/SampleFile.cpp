#include "UselessPch.h"


#pragma warning (disable:4786)
#include "SampleFile.h"
#include "Useless/File/VIFS.h"
#include "Useless/Sound/FileIO/SampleWriter.h"
#include "Useless/Sound/FileIO/SoundFactory.h"
#include "Useless/Sound/Device/SoundCard.h"

namespace Useless {

SampleFile::SampleFile( const std::string &fname )
{
    _file_name = fname;
    
    SPointer<IFile> file = IFS::Instance().OpenFile(_file_name);

    _properties = new Snd::Properties;

    _loader = SoundFactory::Instance().Create( *file, &(*_properties) );

    (*_properties).pitch_ctl  = true;
    (*_properties).volume_ctl = true;

    if ( 1 == (*_properties).channels && Is3DSoundPrefered() )
        { (*_properties).space_ctl = true; }
    else
        { (*_properties).pan_ctl = true; }
}

SampleFile::~SampleFile()
{
}

void SampleFile::GetProperties( Snd::Properties &p ) const
{
    p = *_properties;
}

void SampleFile::Load()
{
    SampleData *data = GetData();
    SampleWriter writer(*data);

    Snd::Format fmt( data->GetProperties() );

    SPointer<IFile> file = IFS::Instance().OpenFile(_file_name);

    (*_loader).Load( *file, writer );
}

};//namespace Useless
