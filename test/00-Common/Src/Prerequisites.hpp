#pragma once

#pragma warning( disable: 4996 )
#include <wx/wx.h>
#include <wx/windowptr.h>

#include <ashespp/Core/Surface.hpp>
#include <ashespp/Core/Instance.hpp>

#include <util/Quaternion.hpp>
#include <util/UtilsDebug.hpp>
#include <util/UtilsDevice.hpp>
#include <util/UtilsInstance.hpp>
#include <util/UtilsSwapChain.hpp>

#include <array>
#include <chrono>

namespace common
{
	static wxSize const WindowSize{ 800, 600 };
	/**
	*\~english
	*\brief
	*	An image data.
	*\~french
	*\brief
	*	Les données d'une image.
	*/
	struct ImageData
	{
		//!\~english	The image dimensions.
		//!\~french		Les dimensions de l'image.
		VkExtent3D size;
		//!\~english	The image buffer.
		//!\~french		Le tampon de l'image.
		ashes::ByteArray data;
		//!\~english	The image pixel format.
		//!\~french		Les format des pixels de l'image.
		VkFormat format;
	};
	/**
	*\~french
	*\brief
	*	Crée une ashes::Surface associant une fenêtre et un GPU.
	*\param[in] window
	*	Fenêtre utilisée pour récupérer les informations dépendantes de la plateforme cible.
	*\param[in] vulkan
	*	L'instance Vulkan.
	*\param[in] gpu
	*	Le GPU choisi pour associer Vulkan à la fenêtre.
	*\~english
	*\brief
	*	Creates a ashes::Surface binding a window to a GPU.
	*\param[in] window
	*	The window used to retrieve the platform dependant informations.
	*\param[in] vulkan
	*	The Vulkan instance.
	*\param[in] gpu
	*	The GPU chosen to bind the window and the GPU.
	*/
	ashes::WindowHandle makeWindowHandle( wxWindow const & window );

	wxString makeName( int index
		, wxString const & name );

	class Application;
	class MainFrame;
}
