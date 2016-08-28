// Copyright (C) 2006  Davis E. King (davis@dlib.net)
// License: Boost Software License   See LICENSE.txt for the full license.
#ifndef DLIB_ALL_SOURCe_
#define DLIB_ALL_SOURCe_

// ISO C++ code
#include "C:/dlib-18.18/dlib/base64/base64_kernel_1.cpp"
#include "C:/dlib-18.18/dlib/bigint/bigint_kernel_1.cpp"
#include "C:/dlib-18.18/dlib/bigint/bigint_kernel_2.cpp"
#include "C:/dlib-18.18/dlib/bit_stream/bit_stream_kernel_1.cpp"
#include "C:/dlib-18.18/dlib/entropy_decoder/entropy_decoder_kernel_1.cpp"
#include "C:/dlib-18.18/dlib/entropy_decoder/entropy_decoder_kernel_2.cpp"
#include "C:/dlib-18.18/dlib/entropy_encoder/entropy_encoder_kernel_1.cpp"
#include "C:/dlib-18.18/dlib/entropy_encoder/entropy_encoder_kernel_2.cpp"
#include "C:/dlib-18.18/dlib/md5/md5_kernel_1.cpp"
#include "C:/dlib-18.18/dlib/tokenizer/tokenizer_kernel_1.cpp"
#include "C:/dlib-18.18/dlib/unicode/unicode.cpp"
#include "C:/dlib-18.18/dlib/data_io/image_dataset_metadata.cpp"

#ifndef DLIB_ISO_CPP_ONLY
// Code that depends on OS specific APIs

// include this first so that it can disable the older version
// of the winsock API when compiled in windows.
#include "C:/dlib-18.18/dlib/sockets/sockets_kernel_1.cpp"
#include "C:/dlib-18.18/dlib/bsp/bsp.cpp"

#include "C:/dlib-18.18/dlib/dir_nav/dir_nav_kernel_1.cpp"
#include "C:/dlib-18.18/dlib/dir_nav/dir_nav_kernel_2.cpp"
#include "C:/dlib-18.18/dlib/dir_nav/dir_nav_extensions.cpp"
#include "C:/dlib-18.18/dlib/linker/linker_kernel_1.cpp"
#include "C:/dlib-18.18/dlib/logger/extra_logger_headers.cpp"
#include "C:/dlib-18.18/dlib/logger/logger_kernel_1.cpp"
#include "C:/dlib-18.18/dlib/logger/logger_config_file.cpp"
#include "C:/dlib-18.18/dlib/misc_api/misc_api_kernel_1.cpp"
#include "C:/dlib-18.18/dlib/misc_api/misc_api_kernel_2.cpp"
#include "C:/dlib-18.18/dlib/sockets/sockets_extensions.cpp"
#include "C:/dlib-18.18/dlib/sockets/sockets_kernel_2.cpp"
#include "C:/dlib-18.18/dlib/sockstreambuf/sockstreambuf.cpp"
#include "C:/dlib-18.18/dlib/sockstreambuf/sockstreambuf_unbuffered.cpp"
#include "C:/dlib-18.18/dlib/server/server_kernel.cpp"
#include "C:/dlib-18.18/dlib/server/server_iostream.cpp"
#include "C:/dlib-18.18/dlib/server/server_http.cpp"
#include "C:/dlib-18.18/dlib/threads/multithreaded_object_extension.cpp"
#include "C:/dlib-18.18/dlib/threads/threaded_object_extension.cpp"
#include "C:/dlib-18.18/dlib/threads/threads_kernel_1.cpp"
#include "C:/dlib-18.18/dlib/threads/threads_kernel_2.cpp"
#include "C:/dlib-18.18/dlib/threads/threads_kernel_shared.cpp"
#include "C:/dlib-18.18/dlib/threads/thread_pool_extension.cpp"
#include "C:/dlib-18.18/dlib/timer/timer.cpp"
#include "C:/dlib-18.18/dlib/stack_trace.cpp"

#ifdef DLIB_PNG_SUPPORT
#include "../image_loader/png_loader.cpp"
#include "../image_saver/save_png.cpp"
#endif

#ifdef DLIB_JPEG_SUPPORT
#include "../image_loader/jpeg_loader.cpp"
#include "../image_saver/save_jpeg.cpp"
#endif

#ifndef DLIB_NO_GUI_SUPPORT
#include "C:/dlib-18.18/dlib/gui_widgets/fonts.cpp"
#include "C:/dlib-18.18/dlib/gui_widgets/widgets.cpp"
#include "C:/dlib-18.18/dlib/gui_widgets/drawable.cpp"
#include "C:/dlib-18.18/dlib/gui_widgets/canvas_drawing.cpp"
#include "C:/dlib-18.18/dlib/gui_widgets/style.cpp"
#include "C:/dlib-18.18/dlib/gui_widgets/base_widgets.cpp"
#include "C:/dlib-18.18/dlib/gui_core/gui_core_kernel_1.cpp"
#include "C:/dlib-18.18/dlib/gui_core/gui_core_kernel_2.cpp"
#endif // DLIB_NO_GUI_SUPPORT

#endif // DLIB_ISO_CPP_ONLY

#endif // DLIB_ALL_SOURCe_
