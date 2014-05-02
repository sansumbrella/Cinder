message(STATUS "Cinder srcs")
set ( srcsAntTweakBar #	${CINDER_ROOT}/src/AntTweakBar/LoadOGLCore.cpp
		      #  ${CINDER_ROOT}/src/AntTweakBar/LoadOGL.cpp
		      #  ${CINDER_ROOT}/src/AntTweakBar/TwBar.cpp
		      #  ${CINDER_ROOT}/src/AntTweakBar/TwColors.cpp
		      #  ${CINDER_ROOT}/src/AntTweakBar/TwFonts.cpp
		      #  ${CINDER_ROOT}/src/AntTweakBar/TwMgr.cpp
		      #  ${CINDER_ROOT}/src/AntTweakBar/TwOpenGLCore.cpp
		      #  ${CINDER_ROOT}/src/AntTweakBar/TwOpenGL.cpp
		      #  ${CINDER_ROOT}/src/AntTweakBar/TwPrecomp.cpp
)


set ( srcsJsonCpp 	#${CINDER_ROOT}/src/jsoncpp/json_reader.cpp
			#${CINDER_ROOT}/src/jsoncpp/json_value.cpp
			#${CINDER_ROOT}/src/jsoncpp/json_writer.cpp
)

			

set ( srcsLineBreak 	${CINDER_ROOT}/src/linebreak/linebreak.c
			${CINDER_ROOT}/src/linebreak/linebreakdata.c
			${CINDER_ROOT}/src/linebreak/linebreakdef.c
)


set ( 	srcsLibTess2	${CINDER_ROOT}/src/libtess2/bucketalloc.c
			${CINDER_ROOT}/src/libtess2/geom.c
			${CINDER_ROOT}/src/libtess2/dict.c
			${CINDER_ROOT}/src/libtess2/mesh.c
			${CINDER_ROOT}/src/libtess2/priorityq.c
			${CINDER_ROOT}/src/libtess2/sweep.c
			${CINDER_ROOT}/src/libtess2/tess.c
)

set (	srcsCinder 	${CINDER_ROOT}/src/cinder/app/AppBasic.cpp
		   	${CINDER_ROOT}/src/cinder/app/App.cpp
		   	${CINDER_ROOT}/src/cinder/app/Window.cpp
		   	${CINDER_ROOT}/src/cinder/app/Renderer.cpp
		   	${CINDER_ROOT}/src/cinder/app/KeyEvent.cpp
		   	${CINDER_ROOT}/src/cinder/app/AppImplLinuxBasic.cpp
		   	${CINDER_ROOT}/src/cinder/app/AppImplLinux.cpp
		   	${CINDER_ROOT}/src/cinder/app/AppImplLinuxRendererGlx.cpp
		   	${CINDER_ROOT}/src/cinder/Area.cpp
			${CINDER_ROOT}/src/cinder/AxisAlignedBox.cpp
			${CINDER_ROOT}/src/cinder/BandedMatrix.cpp
			${CINDER_ROOT}/src/cinder/Base64.cpp
			${CINDER_ROOT}/src/cinder/BSpline.cpp
			${CINDER_ROOT}/src/cinder/BSplineFit.cpp
			${CINDER_ROOT}/src/cinder/Buffer.cpp
			${CINDER_ROOT}/src/cinder/Camera.cpp
			${CINDER_ROOT}/src/cinder/Channel.cpp
			${CINDER_ROOT}/src/cinder/CinderMath.cpp
			${CINDER_ROOT}/src/cinder/Clipboard.cpp
			${CINDER_ROOT}/src/cinder/Color.cpp
			${CINDER_ROOT}/src/cinder/ConvexHull.cpp
			${CINDER_ROOT}/src/cinder/DataSource.cpp
			${CINDER_ROOT}/src/cinder/DataTarget.cpp
			${CINDER_ROOT}/src/cinder/Display.cpp
			${CINDER_ROOT}/src/cinder/Exception.cpp
			${CINDER_ROOT}/src/cinder/Frustum.cpp
			${CINDER_ROOT}/src/cinder/ImageIo.cpp
			${CINDER_ROOT}/src/cinder/ImageSourcePng.cpp
			${CINDER_ROOT}/src/cinder/Matrix.cpp
			${CINDER_ROOT}/src/cinder/ObjLoader.cpp
			${CINDER_ROOT}/src/cinder/Path2d.cpp
			${CINDER_ROOT}/src/cinder/Perlin.cpp
			${CINDER_ROOT}/src/cinder/Plane.cpp
			${CINDER_ROOT}/src/cinder/PolyLine.cpp
			${CINDER_ROOT}/src/cinder/Rand.cpp
			${CINDER_ROOT}/src/cinder/Ray.cpp
			${CINDER_ROOT}/src/cinder/Rect.cpp
			${CINDER_ROOT}/src/cinder/Shape2d.cpp
			${CINDER_ROOT}/src/cinder/Sphere.cpp
			${CINDER_ROOT}/src/cinder/Stream.cpp
			${CINDER_ROOT}/src/cinder/Surface.cpp
			${CINDER_ROOT}/src/cinder/Text.cpp
			${CINDER_ROOT}/src/cinder/Timeline.cpp
			${CINDER_ROOT}/src/cinder/TimelineItem.cpp
			${CINDER_ROOT}/src/cinder/Triangulate.cpp
			${CINDER_ROOT}/src/cinder/Timer.cpp
			${CINDER_ROOT}/src/cinder/TriMesh.cpp
			${CINDER_ROOT}/src/cinder/Tween.cpp
			${CINDER_ROOT}/src/cinder/Unicode.cpp
			${CINDER_ROOT}/src/cinder/Xml.cpp
			${CINDER_ROOT}/src/cinder/Utilities.cpp
			${CINDER_ROOT}/src/cinder/Url.cpp
			${CINDER_ROOT}/src/cinder/Display.cpp
#			${CINDER_ROOT}/src/cinder/Json.cpp
			${CINDER_ROOT}/src/cinder/gl/DisplayList.cpp
			${CINDER_ROOT}/src/cinder/gl/GLee.c
			${CINDER_ROOT}/src/cinder/gl/Material.cpp
			${CINDER_ROOT}/src/cinder/gl/Texture.cpp
			${CINDER_ROOT}/src/cinder/gl/TileRender.cpp
			${CINDER_ROOT}/src/cinder/gl/Light.cpp
			${CINDER_ROOT}/src/cinder/gl/gl.cpp
			${CINDER_ROOT}/src/cinder/gl/Vbo.cpp
			${CINDER_ROOT}/src/cinder/gl/GlslProg.cpp
			${CINDER_ROOT}/src/cinder/gl/StereoAutoFocuser.cpp
			${CINDER_ROOT}/src/cinder/gl/TextureFont.cpp
			${CINDER_ROOT}/src/cinder/gl/Fbo.cpp
			${CINDER_ROOT}/src/cinder/Font.cpp
			${CINDER_ROOT}/src/cinder/ip/Blend.cpp
			${CINDER_ROOT}/src/cinder/ip/Fill.cpp
			${CINDER_ROOT}/src/cinder/ip/Grayscale.cpp
			${CINDER_ROOT}/src/cinder/ip/Premultiply.cpp
			${CINDER_ROOT}/src/cinder/ip/Threshold.cpp
			${CINDER_ROOT}/src/cinder/ip/EdgeDetect.cpp
			${CINDER_ROOT}/src/cinder/ip/Flip.cpp
			${CINDER_ROOT}/src/cinder/ip/Hdr.cpp
			${CINDER_ROOT}/src/cinder/ip/Resize.cpp
			${CINDER_ROOT}/src/cinder/ip/Trim.cpp
			${CINDER_ROOT}/src/cinder/svg/Svg.cpp
            ${CINDER_ROOT}/src/cinder/ImageSourceFileFreeImage.cpp
#			${CINDER_ROOT}/src/cinder/params/Params.cpp
   )
