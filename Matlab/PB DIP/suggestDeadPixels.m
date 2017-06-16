## Author: Patrizio Bertoni <xnext@xnext-e5>
## Created: 2017-06-15

function [deadPixels] = suggestDeadPixels (matInt, lightTol)

  pxMeans = mean(matInt, 2);
  deadPixels = find(pxMeans < lightTol);

endfunction
