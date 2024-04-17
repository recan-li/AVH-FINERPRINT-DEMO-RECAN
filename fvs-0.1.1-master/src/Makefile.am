
lib_LTLIBRARIES = libfvs.la

LIBRARY_CURRENT   = @LIBRARY_CURRENT@
LIBRARY_REVISION  = @LIBRARY_REVISION@
LIBRARY_AGE       = @LIBRARY_AGE@

libfvs_la_SOURCES = image.c \
		    import.c \
		    export.c \
		    floatfield.c \
		    file.c \
		    img_base.c \
		    img_enhance1.c \
		    histogram.c \
		    imagemanip.c \
		    minutia.c \
		    img_morphology.c \
		    version.c \
		    matching.c

libfvs_la_LDFLAGS = -g `Magick-config --ldflags --libs` \
		    -version-info $(LIBRARY_CURRENT):$(LIBRARY_REVISION):$(LIBRARY_AGE)

INCLUDES = -I../include

CLEANFILES = core *.core *~ libfvs.la

