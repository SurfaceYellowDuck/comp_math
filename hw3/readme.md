# Сингулярное разложение и сжатие изображений

Сжатие и восстановление производилось четырьмя способами.


Условия эксперимента: размер промежуточного представления в два раза меньше исходного изображения,при сжатии параметр k 
изменяется динамически, в зависимости от того, какой размер картинки и во сколько раз необходимо сжать изображение.
    

    -numpy method:

<div style="display: flex; justify-content: center;">
    <img src="./pictures/original/sample_640x426.bmp" alt="Image 1"  width="300" height="300" style="margin-right: 10px;">
    <img src="./pictures/decompressed/360-426-np_decompressed.bmp" alt="Image 1" width="300" height="300" style="margin-right: 10px;">
</div>

<div style="display: flex; justify-content: center;">
    <img src="./pictures/original/360-360-sample.bmp" alt="Image 1"  width="300" height="300" style="margin-right: 10px;">
    <img src="./pictures/decompressed/360-360-sample-np_decompressed.bmp" alt="Image 1" width="300" height="300" style="margin-right: 10px;">
</div>

    - Power method: 

<div style="display: flex; justify-content: center;">
    <img src="./pictures/original/sample_640x426.bmp" alt="Image 1"  width="300" height="300" style="margin-right: 10px;">
    <img src="./pictures/decompressed/360-426-power_decompressed.bmp" alt="Image 1" width="300" height="300" style="margin-right: 10px;">
</div>

<div style="display: flex; justify-content: center;">
    <img src="./pictures/original/360-360-sample.bmp" alt="Image 1"  width="300" height="300" style="margin-right: 10px;">
    <img src="./pictures/decompressed/360-360-power_decompressed.bmp" alt="Image 1" width="300" height="300" style="margin-right: 10px;">
</div>

    - Householder method: 
<div style="display: flex; justify-content: center;">
    <img src="./pictures/original/sample_640x426.bmp" alt="Image 1"  width="300" height="300" style="margin-right: 10px;">
    <img src="./pictures/decompressed/sample_640x426-householder_decompressed.bmp" alt="Image 1" width="300" height="300" style="margin-right: 10px;">
</div>
<div style="display: flex; justify-content: center;">
    <img src="./pictures/original/360-360-sample.bmp" alt="Image 1"  width="300" height="300" style="margin-right: 10px;">
    <img src="./pictures/decompressed/360-360-householder_decompressed.bmp" alt="Image 1" width="300" height="300" style="margin-right: 10px;">
</div>

    - Jacobi method:
<div style="display: flex; justify-content: center;">
    <img src="./pictures/original/sample_640x426.bmp" alt="Image 1"  width="300" height="300" style="margin-right: 10px;">
    <img src="./pictures/decompressed/sample_640x426-jacobi_decompressed.bmp" alt="Image 1" width="300" height="300" style="margin-right: 10px;">
</div>
<div style="display: flex; justify-content: center;">
    <img src="./pictures/original/360-360-sample.bmp" alt="Image 1"  width="300" height="300" style="margin-right: 10px;">
    <img src="./pictures/decompressed/360-360-jacobi_decompressed.bmp" alt="Image 1" width="300" height="300" style="margin-right: 10px;">
</div>

Из трёх методов сжатия изображений особенно хорошо показал себя метод Хаусхолдера. Он позволяет сравнительно быстро сжать
изображение, не уступая в качестве библиотеке numpy на представленных изображениях. Power метод не уступает алгоритму 
Хаусхолдера в качестве, хоть и является примитивным, но сингулярное разложение он строит гораздо дольше. Самая плохая
реализация получилась с методом Якоби. Он гораздо медленнее приближает, и качество восстановленного изображения после
сжатия оставляет желать лучшего.

В ходе реализации были использованы следующие источники:

https://www.grad.hr/nastava/gs/prg/NumericalRecipesinC.pdf


https://drsfenner.org/blog/2016/03/svd-computation-capstone/