#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
using namespace std;
int main(){
    int n = 4, m = 2; //под размерность данных (в "исключающем или" 4 возможные пары)
    double lErr;
    double outer, //выходной нейрон
           *enters, //входы
           *hidden, //нейроны на скрытом слое

           /*
            * нужен массив коэффициентов (весов) межнейронных
            * связей это будет двухмерный массив -
            * - 1 индекс: от какого нейрона,
            * 2 индекс: к какому
           */
           **wEH, //neuron from enter to hidden

           /*
            * аналогично между скрытым и выходным слоем
            * но т.к. на выходе только один, то будет
            * массив одномерный
           */
           *wHO, //neuron from hidden to outer
           *answers, //под массив ответов
           **patterns; //под массив шаблонов
    
    /*
    * создаём массивы шаблонных данных
    * для обучения
    */
    patterns = new double*[n];
    for(int i = 0; i < n; i++)
        patterns[i] = new double[m];
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            cin >> patterns[i][j];
        }
    }
    answers = new double[n];
    for(int i = 0; i < n; i++)
        cin >> answers[i];

    /*
    * на вход должны подаваться данные, соразмерные
    * требуемым, поэтому выбираем такую длину,
    * которая совпадает с длиной массива из
    * входных данных, а так как она одинакова
    * для любой пары входных данных, то возьмём,
    * без о.о., длину первой пары
    */
    enters = new double[m];

    /*
    * количество скрытых нейронов надо определить
    * жёстко, потому что выбор количества
    * скрытых нейронов, это сложная задача,
    * которая решается по-хорошему имперически,
    * поэтому после нескольких прогонов взяли
    * именно это значение
    */
    hidden = new double[2];

    /*
    * размер массивов связей будет зависеть от
    * размеров слоёв
    */
    wEH = new double*[m];
    for(int i = 0; i < m; i++)
        wEH[i] = new double[2];

    wHO = new double[2];

    /*
    * процедура инициализации, которая подразумевает
    * инициализацию весовых коэффициентов небольшими
    * случаными значениями
    */

    /*
    * между входами и скрытым слоем
    */
    for(int i = 0; i < m; i++){
        for(int j = 0; j < 2; j++){
            /*
            * нужно число больше нуля, меньше единицы
            * и близкое к нулю
            */
            wEH[i][j] = 0.00001 * (rand() % 101);
        }
    }
    /*
    * между скрытым и выходным
    */
    for(int i = 0; i < 2; i++){
        wHO[i] = 0.00001 * (rand() % 101);
    }

    /*
    * массив для ошибок на скрытом слое
    */
    double *err = new double[2];
        
    /*
    * глобальная ошибка
    */
    double gError = 0;
      
    /*
    * непосредственно процедура обучения
    * которая должна длиться до требуемого
    * результата (ошибка равна нулю)
    */
    do{
        gError = 0;
        for(int p = 0; p < n; p++){
            /*
            * передаём значения на вход
            */
            for(int i = 0; i < m; i++)
               enters[i] = patterns[p][i];
             
            /*
            * считаем выход
            */
            
            /*
            * расчёт первого слоя
            */
            for(int i = 0; i < 2; i++){
                hidden[i] = 0;
                for(int j = 0; j < m; j++){
                    hidden[i] += enters[j] * wEH[j][i];
                }
                if(hidden[i] > 0.5)
                    hidden[i] = 1;
                else
                    hidden[i] = 0;
            }
            /*
            * расчёт второго слоя
            */
            outer = 0;
            for(int i = 0; i < 2; i++){
                outer += hidden[i] * wHO[i];
            }
            if(outer > 0.5)
                outer = 1;
            else
                outer = 0;
            
            /*
            * считаем локальную ошибку
            */
            lErr = answers[p] - outer;
            gError += fabs(lErr);
            
            /*
            * расчёт ошибок скрытого слоя (здесь-то
            * и применяем метод обратного
            * распространения ошибки)
            */
            for(int i = 0; i < 2; i++)
                err[i] = lErr * wHO[i];
            
            /*
            * корректируем весовые коэффициенты
            * (можно слева направо, а можно и
            * наоборот)
            */
            for(int i = 0; i < m; i++){
                for(int j = 0; j < 2; j++){
                    wEH[i][j] += 0.1 * err[j] * enters[i];
                }
            }
            for(int i = 0; i < 2; i++)
                wHO[i] += 0.1 * lErr * hidden[i];
        }
    }
    while(gError != 0);
    
    cout << "\n";
    cout << "Result:\n";
    
    for(int p = 0; p < n; p++){
        for(int i = 0; i < m; i++)
            enters[i] = patterns[p][i];

        for(int i = 0; i < 2; i++){
            hidden[i] = 0;
            for(int j = 0; j < m; j++){
                hidden[i] += enters[j] * wEH[j][i];
            }
            if(hidden[i] > 0.5)
                hidden[i] = 1;
            else
                hidden[i] = 0;
        }
        outer = 0;
        for(int i = 0; i < 2; i++){
            outer += hidden[i] * wHO[i];
        }
        if(outer > 0.5)
            outer = 1;
        else
            outer = 0;
                
        cout << outer << endl;
    }

    return 0;
}
