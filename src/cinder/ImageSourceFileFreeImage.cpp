#include "cinder/ImageSourceFileFreeImage.h"
#include "cinder/Utilities.h"

namespace cinder {

//  FreeImage IO implemented with IStream
static unsigned _ReadProc(void *buffer, unsigned size, unsigned count, fi_handle handle) 
{
	IStreamFile* stream = static_cast<IStreamFile*>(handle);
	return stream->readDataAvailable(buffer, count*size);
}

static unsigned _WriteProc(void *buffer, unsigned size, unsigned count, fi_handle handle) 
{
	//  Writing not supported on input stream
	return size;
}

static int _SeekProc(fi_handle handle, long offset, int origin) 
{
	assert(origin != SEEK_END);

	IStreamFile* stream = static_cast<IStreamFile*>(handle);

	if (origin == SEEK_SET) {
		stream->seekAbsolute(offset);
	} else {
		stream->seekRelative(offset);
	}

	return 0;
}

static long _TellProc(fi_handle handle) 
{
	IStreamFile* stream = static_cast<IStreamFile*>(handle);
	return stream->tell();
}

// ----------------------------------------------------------


ImageSourceFileFreeImageRef ImageSourceFileFreeImage::createFileFreeImageRef( DataSourceRef dataSourceRef, ImageSource::Options options )
{
	return ImageSourceFileFreeImageRef( new ImageSourceFileFreeImage( dataSourceRef, options ) );
}

ImageSourceFileFreeImage::ImageSourceFileFreeImage( DataSourceRef dataSourceRef, ImageSource::Options options )
	: ImageSource(), mBitmap(NULL)
{
	static const char* imageTypes[] = {
		"FIT_UNKNOWN",
		"FIT_BITMAP ",
		"FIT_UINT16 ",
		"FIT_INT16  ",
		"FIT_UINT32 ",
		"FIT_INT32  ",
		"FIT_FLOAT  ",
		"FIT_DOUBLE ",
		"FIT_COMPLEX",
		"FIT_RGB16  ",
		"FIT_RGBA16 ",
		"FIT_RGBF   ",
		"FIT_RGBAF  ",
	};
	static const char* colorTypes[] = {
		"FIC_MINISWHITE",
		"FIC_MINISBLACK",
		"FIC_RGB       ",
		"FIC_PALETTE   ",
		"FIC_RGBALPHA  ",
		"FIC_CMYK      ",
	};

	FreeImageIO io;

	io.read_proc  = _ReadProc;
	io.write_proc = _WriteProc;
	io.tell_proc  = _TellProc;
	io.seek_proc  = _SeekProc;

	IStreamRef stream = dataSourceRef->createStream();

	FREE_IMAGE_FORMAT format = FreeImage_GetFileTypeFromHandle(&io, stream.get());
    std::cout<<"FreeImage format detected: " << format << std::endl;

	mBitmap = FreeImage_LoadFromHandle(format, &io, stream.get());
	mWidth  = FreeImage_GetWidth(mBitmap);
	mHeight = FreeImage_GetHeight(mBitmap);
    std::cout<<"ImageSourceFileFreeImage loaded from : " <<  stream->getFileName().c_str() <<
			" W " << mWidth << " H " << mHeight << std::endl;

	FREE_IMAGE_TYPE imageType = FreeImage_GetImageType(mBitmap);
	FREE_IMAGE_COLOR_TYPE colorType = FreeImage_GetColorType(mBitmap);
	// bits per pixel
	unsigned int bpp = FreeImage_GetBPP(mBitmap);
    std::cout << "Image type " << imageTypes[imageType] << " color type " << colorTypes[colorType] << " bpp " << bpp << std::endl;
	//  row data
	mRowBytes = FreeImage_GetLine(mBitmap);
	mRowPitch = FreeImage_GetPitch(mBitmap);
    std::cout << " Row width " <<  mRowBytes << " row pitch " << mRowPitch << std::endl;

    if (imageType == FIT_BITMAP) {
        setDataType( ImageIo::UINT8 );
    }
    else if (imageType == FIT_UINT16 || imageType == FIT_RGB16 || imageType == FIT_RGBA16) {
        setDataType( ImageIo::UINT16 );
    }
    else if (imageType == FIT_FLOAT || imageType == FIT_RGBF || imageType == FIT_RGBAF) {
        setDataType( ImageIo::FLOAT32 );
    }
    else {
        setDataType( DATA_UNKNOWN );
    }

	//  XXX Channel order for CM_RGB and CM_RGBA is probably only correct for data type UINT8
    if (colorType == FIC_RGB) {
        setColorModel( ImageIo::CM_RGB );
        //  if bpp == 32 then RGB is padded with an alpha value
        setChannelOrder( imageType == FIT_BITMAP && bpp == 32 ? ImageIo::BGRX : ImageIo::BGR );
    }
    else if (colorType == FIC_RGBALPHA) {
        setColorModel( ImageIo::CM_RGB );
        setChannelOrder( ImageIo::BGRA );
    }
    else if (colorType == FIC_MINISBLACK ) {
        setColorModel( ImageIo::CM_GRAY );
        setChannelOrder( ImageIo::Y );
    }
    else {
        setColorModel( ImageIo::CM_UNKNOWN );
        setChannelOrder( ImageIo::CUSTOM );
    }
}

void ImageSourceFileFreeImage::load( ImageTargetRef target )
{
	// get a pointer to the ImageSource function appropriate for handling our data configuration
	ImageSource::RowFunc func = setupRowFunc( target );
	
	for( int32_t row = 0; row < mHeight; ++row ) {
        const uint8_t *data = FreeImage_GetScanLine(mBitmap, mHeight-row-1);
		((*this).*func)( target, row, data );
	}
}

void ImageSourceFileFreeImage::registerSelf()
{
	const int32_t SOURCE_PRIORITY = 2;
	
	// XXX find a way to deinit
    FreeImage_Initialise(true);
	ImageIoRegistrar::registerSourceGeneric( ImageSourceFileFreeImage::createRef, SOURCE_PRIORITY );
}

ImageSourceFileFreeImage::~ImageSourceFileFreeImage()
{
	if (mBitmap) {
		FreeImage_Unload(mBitmap);
		mBitmap = NULL;
	}
}

} // namespace ci
