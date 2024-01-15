## Author: Patrizio Bertoni <xnext@xnext-e5>
## Created: 2017-06-15

function [edges] = suggestEdgeDetection (matInt)

diffs = diff(matInt');
diffs = diffs';

edges = diffs;

endfunction
