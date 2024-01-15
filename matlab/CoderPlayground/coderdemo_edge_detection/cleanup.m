if isempty(currentFigures), currentFigures = []; end;
close(setdiff(findall(0, 'type', 'figure'), currentFigures))
clear mex
delete *.mexa64
[~,~,~] = rmdir('/media/xnext/DATA/Software/xspectra-ui/code-playground/Matlab/SimpleCoderExample/coderdemo_edge_detection/codegen','s');
clear /media/xnext/DATA/Software/xspectra-ui/code-playground/Matlab/SimpleCoderExample/coderdemo_edge_detection/sobel.m
delete /media/xnext/DATA/Software/xspectra-ui/code-playground/Matlab/SimpleCoderExample/coderdemo_edge_detection/sobel.m
delete /media/xnext/DATA/Software/xspectra-ui/code-playground/Matlab/SimpleCoderExample/coderdemo_edge_detection/hello.jpg
clear
load old_workspace
delete old_workspace.mat
delete /media/xnext/DATA/Software/xspectra-ui/code-playground/Matlab/SimpleCoderExample/coderdemo_edge_detection/cleanup.m
cd /media/xnext/DATA/Software/xspectra-ui/code-playground/Matlab/SimpleCoderExample
rmdir('/media/xnext/DATA/Software/xspectra-ui/code-playground/Matlab/SimpleCoderExample/coderdemo_edge_detection','s');
