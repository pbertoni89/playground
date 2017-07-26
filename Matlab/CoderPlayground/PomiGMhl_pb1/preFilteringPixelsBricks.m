function [ oData ] = preFilteringPixelsBricks(iData, pixels, bins)
%preFilteringPixelsBricks
%  pixels - interval of interest (exclude belt and roof)
%  bricks - interval of interest

	oData = iData(pixels, :, bins);

end

