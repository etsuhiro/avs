/*! @file Wd3xImageCanvas.h */
#pragma	once

#include "Wd3xImageTag.h"
#include "../avs/ImageCanvas.h"

namespace image {
	class Wd3xImageCanvas : public ImageCanvas {
	public:
		Wd3xImageCanvas( avs::Wd3xImageTag* tag, int n_layer )
			: ImageCanvas(tag,n_layer)
			, m_pW3dxImageTag(tag)
		{}

		avs::Wd3xImageTag& layer_wd3x(int n) const { return m_pW3dxImageTag[n]; }

	private:
		avs::Wd3xImageTag*	m_pW3dxImageTag;

	};
}
