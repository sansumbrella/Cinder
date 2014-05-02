#pragma once

#include "cinder/Cinder.h"
#include "cinder/ImageIo.h"
#include "cinder/Exception.h"

#include "FreeImage.h"

namespace cinder {

typedef std::shared_ptr<class ImageSourceFileFreeImage>	ImageSourceFileFreeImageRef;

class ImageSourceFileFreeImage : public ImageSource {
  public:
	static ImageSourceRef				createRef( DataSourceRef dataSourceRef, ImageSource::Options options = ImageSource::Options() ) { return createFileFreeImageRef( dataSourceRef, options ); }
	static ImageSourceFileFreeImageRef	createFileFreeImageRef( DataSourceRef dataSourceRef, ImageSource::Options options = ImageSource::Options() );

	virtual void	load( ImageTargetRef target );

	static void registerSelf();

	virtual ~ImageSourceFileFreeImage();

  protected:
	ImageSourceFileFreeImage( DataSourceRef dataSourceRef, ImageSource::Options options );
	
	std::shared_ptr<uint8_t>	mData;
	int32_t						mRowBytes;
	int32_t						mRowPitch;
	FIBITMAP*					mBitmap;
};

REGISTER_IMAGE_IO_FILE_HANDLER( ImageSourceFileFreeImage )

class ImageSourceFileFreeImageExceptionUnsupportedData : public ImageIoException {
};

} // namespace ci

