close all;
%% Seed location is already filled
% bw = false(3, 4);
% bw(2, 1) = true;
% 
% [out, cnt] = fastfloodfill(1, 2, bw);
% figure;
% subplot(131); imagesc(bw); axis image; colorbar;
% subplot(132); imagesc(out); axis image; colorbar;
% subplot(133); imagesc(cnt); axis image; colorbar;

%% Empty input
% bw = false(3, 4);
% 
% [out, cnt] = fastfloodfill(1, 2, bw);
% figure;
% subplot(131); imagesc(bw); axis image; colorbar;
% subplot(132); imagesc(out); axis image; colorbar;
% subplot(133); imagesc(cnt); axis image; colorbar;

%% A diagonal edge
% bw = logical(eye(5, 6));
% 
% [out, cnt] = fastfloodfill(1, 2, bw);
% figure;
% subplot(131); imagesc(bw); axis image; colorbar;
% subplot(132); imagesc(out); axis image; colorbar;
% subplot(133); imagesc(cnt); axis image; colorbar;

%% Maze
img = imread('maze.png');
bw = ~logical(rgb2gray(img));

[out, cnt] = fastfloodfill(20, 20, bw);

figure;
subplot(131); imagesc(bw); axis image; colorbar;
subplot(132); imagesc(out); axis image; colorbar;
subplot(133); imagesc(cnt); axis image; colorbar;

%%
% s = 1;
% img = zeros(300*s, 200*s, 'uint8');
% img = insertShape(img, 'circle', ...
%     [99*s 145*s 90*s; 50*s 80*s 28*s; 150*s 78*s 28*s; 100*s 145*s 20*s]);
% img = insertMarker(img, [100*s 198*s], 'x', 'size', 20*s);
% bw = logical(rgb2gray(img));
% 
% [out, cnt] = fastfloodfill(1, 1, bw);
% figure;
% subplot(131); imagesc(bw); axis image; colorbar;
% subplot(132); imagesc(out); axis image; colorbar;
% subplot(133); imagesc(cnt); axis image; colorbar;
% 
% [out, cnt] = fastfloodfill(100*s, 145*s, bw);
% figure;
% subplot(131); imagesc(bw); axis image; colorbar;
% subplot(132); imagesc(out); axis image; colorbar;
% subplot(133); imagesc(cnt); axis image; colorbar;
% 
% [out, cnt] = fastfloodfill(60*s, 145*s, bw);
% figure;
% subplot(131); imagesc(bw); axis image; colorbar;
% subplot(132); imagesc(out); axis image; colorbar;
% subplot(133); imagesc(cnt); axis image; colorbar;

%%
% s = 100;
% img = zeros(300*s, 200*s, 'uint8');
% img = insertShape(img, 'circle', ...
%     [99*s 145*s 90*s; 50*s 80*s 28*s; 150*s 78*s 28*s; 100*s 145*s 20*s]);
% img = insertMarker(img, [100*s 198*s], 'x', 'size', 20*s);
% bw = logical(rgb2gray(img));
% 
% tic
% [out, cnt] = fastfloodfill(60*s, 145*s, bw);
% toc
% 
% figure;
% subplot(131); imagesc(imresize(bw, 1/s, 'nearest')); axis image; colorbar;
% subplot(132); imagesc(imresize(out, 1/s, 'nearest')); axis image; colorbar;
% subplot(133); imagesc(imresize(cnt, 1/s, 'nearest')); axis image; colorbar;

