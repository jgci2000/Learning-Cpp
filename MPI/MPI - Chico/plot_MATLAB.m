clc
clear all
close all

% Este script MATLAB permite importar o 
% ficheiro bina'rio produzido pelos programas em C
% e produzir um plot. Pode ser muito util para
% ver se as condicoes fronteira estao corretas,
% para ve se nao ha' erros nas fronteiras dos
% subdominios, etc.

M=100;

fileID = fopen('array2d_a.bin');
array_MPI = fread(fileID, [M M],'double');
fclose(fileID);

% Para o MATLAB, o 2º elemento do ficheiro e' o 
% primeiro elemento da segunda linha: e' necessario transpor
% para que a disposicao da matriz seja igual `a do C:
array_MPI=array_MPI';
% Assim, as linhas e colunas sao iguais `as do programa em C

L=1;
x=linspace(-L,L,M);
y=linspace(-L,L,M);

% Mas como para as figuras do MATLAB,
% o avanço numa linha e' um aumento de x,
% para a figura ficar consistente com o programa em C,
% tem que se transpor outra vez!

figure
mesh(x,y,array_MPI')
xlim([-L L])
ylim([-L L])
xlabel('\it{x}')
ylabel('\it{y}')
title("array\_MPI")


