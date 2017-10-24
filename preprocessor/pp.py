import sys
import numpy as np
import cv2

#atencao, o codigo esta removendo apenas as bordas pretas e na horizontal
#para remover as bordas na vertical o processo eh o mesmo, basta
#adcionar/mudar os parametros de corte.

def main():
    cap = cv2.VideoCapture('2001od.mp4')
    x = int(cap.get(4))
    y = int(cap.get(3))
    size = (x,y)
    fourcc = cv2.VideoWriter_fourcc(*'DIVX')
    out = cv2.VideoWriter('output.avi', fourcc, 20.0, (size))
    #matriz para acumular a soma para derterminar onde as bordas devem ser removidas   
    matrix = np.zeros(size, dtype=np.uint64)
    #imagem para acumular intensidades
    sum_img = np.zeros(size, dtype=np.uint64)
    #inicializacao como matriz para evitar bug
    img_grey = np.zeros(size, dtype=np.uint64)

    while(cap.isOpened()):
        ret, frame = cap.read()

        #converte o frame para escala de cinza
        img_grey = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

        #filtro de canny
        #o primeiro parametro eh a imagem
        #o segundo eh o valor minimo da intensidade pra ser considerado borda
        #o terceiro eh o valor maximo da intensidade pra ser considerado borda
        edges = cv2.Canny(img_grey,50,200)
        cv2.imshow('CANNY',edges)
        #soma as intensidades da imagem atual com a anterior
        sum_img += edges
        # joga na matriz
        matrix = sum_img
        #soma cada linha da imagem e coloca em um vetor.
        #cada elemento representa a soma de uma linha
        matrix_sum_x = matrix.sum(axis=1)
        #corte da parte de cima
        #arg1: o vetor das somas
        #arg2: a posicao minima. neste caso, comeca em 0
        #arg3: a posicao maxima, neste caso, o meio
        cut1 = cut_top(matrix_sum_x, 0, x/2)
        #corte da parte de baixo
        #o funcionamento aqui eh muito parecido com o de cima, porem
        #comecamos do final da array e vamos ate a metade
        cut2 = cut_bottom(matrix_sum_x, x/2, x)

        #realiza o corte
        img_crop = img_grey[cut1:cut2,0:639]

        #aplica filtro para remover ruidos
        img_filter = cv2.medianBlur(img_crop, 5)
        #equaliza o histograma
        final = cv2.equalizeHist(img_filter);
        cv2.imshow('PP',final)
        out.write(cv2.cvtColor(final, cv2.COLOR_GRAY2BGR))
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    # matrix_sum_y = matrix.sum(axis=0)
    matrix_sum_x = matrix.sum(axis=1)

    # posicao do corte de cima
    cut1 = cut_top(matrix_sum_x, 0, 180)
    # posicao do corte de baixo
    cut2 = cut_bottom(matrix_sum_x, 180, 360)

    #salva arquivo para debug
    save_sum(matrix_sum_x)
    #salva arquivo para debug
    save_matrix(matrix)

    # Release everything if job is finished
    cap.release()   
    out.release()
    cv2.destroyAllWindows()


def save_matrix (matrix):
    np.savetxt('matrix.txt', matrix, fmt='%d')

def save_sum (data):
    np.savetxt('sum.txt', data, fmt='%64d')

def cut_top(data, min, max):
    cut_pos = 0
    for pos in range(min, max):
        if (data[pos] == 0):
            cut_pos = pos
    return cut_pos

#a direfenca eh que faz um loop inverso, do final ate a metade da array
def cut_bottom(data, min, max):
    cut_pos = 0
    for pos in reversed(range(min, max)):
        if (data[pos] == 0):
            cut_pos = pos
    return cut_pos




if __name__ == '__main__':
    main()