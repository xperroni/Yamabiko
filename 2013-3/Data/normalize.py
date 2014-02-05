
from os.path import join as joinpath
from shutil import copyfile

folder1 = '/home/helio/Projects/Data/Landmarks/2013-12-03-01'
folder2 = '/home/helio/Projects/Data/Landmarks/2013-12-03-01-normal'

name_png = 'stop-%d-%d-%d-still.png'
name_txt = 'stop-%d-%d-%d-position.txt'

for i in range(0, 12):
    for j in range(0, 10):
        for k in range(0, 13):
            u = j if i % 2 == 0 else (9 - j)
            v = 11 - i
            t = k if j % 2 == 0 else (12 - k)
            
            path_png_1 = joinpath(folder1, name_png % (i, j, k))
            path_png_2 = joinpath(folder2, name_png % (u, v, t))

            path_txt_1 = joinpath(folder1, name_txt % (i, j, k))
            path_txt_2 = joinpath(folder2, name_txt % (u, v, t))
            
            copyfile(path_png_1, path_png_2)
            print path_png_1
            print path_png_2
            print ''

            copyfile(path_txt_1, path_txt_2)
            print path_txt_1
            print path_txt_2
            print ''

