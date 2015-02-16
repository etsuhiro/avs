#ifndef avs_image_canvas_h_
#define avs_image_canvas_h_

#include "ImageTag.h"

namespace avs {
	class ImageCanvas {
	public:
		ImageCanvas(avs::ImageTag* tag, int n_layer) : m_pImageTag(tag), m_nLayer(n_layer) {}

		int getMaxLayer() const { return m_nLayer; }
		avs::ImageTag& layer(int n) const { return m_pImageTag[n]; }

	private:
		avs::ImageTag*	m_pImageTag;	// ImageTagへのポインタ	
		int				m_nLayer;		// レイヤー数＝ImageTagの数

	};
}

#endif
