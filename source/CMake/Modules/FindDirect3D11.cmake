if( WIN32 )
	FIND_PATH(D3D11_ROOT_DIR Include/d3d11.h
	  HINTS
	    ENV DXSDK_DIR
	  PATH_SUFFIXES  DxSDK_Jun2010
	)

	FIND_PATH(D3D11_INCLUDE_DIR d3d11.h 
	  HINTS
	  PATH_SUFFIXES Include
	  PATHS
	  ${D3D11_ROOT_DIR}
	)

	if( CMAKE_CL_64 )
		FIND_PATH(D3D11_LIBRARY_DIR d3d11.lib
		  HINTS
		  PATH_SUFFIXES Lib/x64
		  PATHS
		  ${D3D11_ROOT_DIR}
		)

		FIND_LIBRARY(D3D11_LIBRARY
		  NAMES d3d11.lib
		  HINTS
		  PATH_SUFFIXES Lib/x64
		  PATHS
		  ${D3D11_LIBRARY_DIR}
		)

		FIND_LIBRARY(D3D11_X_LIBRARY
		  NAMES d3dx11.lib
		  HINTS
		  PATH_SUFFIXES Lib/x64
		  PATHS
		  ${D3D11_LIBRARY_DIR}
		)

		FIND_LIBRARY(D3D11_DXGI_LIBRARY
		  NAMES dxgi.lib
		  HINTS
		  PATH_SUFFIXES Lib/x64
		  PATHS
		  ${D3D11_LIBRARY_DIR}
		)

		FIND_LIBRARY(D3D11_D3DCOMPILER_LIBRARY
		  NAMES d3dcompiler.lib
		  HINTS
		  PATH_SUFFIXES Lib/x64
		  PATHS
		  ${D3D11_LIBRARY_DIR}
		)
	else()
		FIND_PATH(D3D11_LIBRARY_DIR d3d11.lib
		  HINTS
		  PATH_SUFFIXES Lib/x86
		  PATHS
		  ${D3D9_ROOT_DIR}
		)

		FIND_LIBRARY(D3D11_LIBRARY
		  NAMES d3d11.lib
		  HINTS
		  PATH_SUFFIXES Lib/x86
		  PATHS
		  ${D3D11_LIBRARY_DIR}
		)

		FIND_LIBRARY(D3D11_X_LIBRARY
		  NAMES d3dx11.lib
		  HINTS
		  PATH_SUFFIXES Lib/x86
		  PATHS
		  ${D3D11_LIBRARY_DIR}
		)

		FIND_LIBRARY(D3D11_DXGI_LIBRARY
		  NAMES dxgi.lib
		  HINTS
		  PATH_SUFFIXES Lib/x86
		  PATHS
		  ${D3D11_LIBRARY_DIR}
		)

		FIND_LIBRARY(D3D11_D3DCOMPILER_LIBRARY
		  NAMES d3dcompiler.lib
		  HINTS
		  PATH_SUFFIXES Lib/x86
		  PATHS
		  ${D3D11_LIBRARY_DIR}
		)
	endif()

	MARK_AS_ADVANCED( D3D11_LIBRARY_DIR D3D11_LIBRARY D3D11_X_LIBRARY D3D11_DXGI_LIBRARY D3D11_D3DCOMPILER_LIBRARY )
	find_package_handle_standard_args( Direct3D11  DEFAULT_MSG D3D11_X_LIBRARY D3D11_DXGI_LIBRARY D3D11_LIBRARY D3D11_D3DCOMPILER_LIBRARY D3D11_INCLUDE_DIR )
	if ( DIRECT3D11_FOUND )
		set( D3D11_LIBRARIES ${D3D11_LIBRARY} ${D3D11_X_LIBRARY} ${D3D11_DXGI_LIBRARY} ${D3D11_D3DCOMPILER_LIBRARY} CACHE STRING "Direct3D11 libraries" )
	endif()
endif()