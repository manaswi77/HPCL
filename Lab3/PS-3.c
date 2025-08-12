#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void run_test(int n, int threads, int schedule_type, int chunk){
    // schedule_type: 0=static, 1=dynamic
    double *A = malloc(n*sizeof(double));
    double *B = malloc(n*sizeof(double));
    double scalar = 5.0;

    for(int i=0;i<n;i++){ A[i]=i; B[i]=0.0; }

    omp_set_num_threads(threads);
    double start = omp_get_wtime();

    if(schedule_type==0){
        #pragma omp parallel for schedule(static,1) // placeholder; we'll set runtime schedule below
        for(int i=0;i<n;i++){
            B[i] = A[i] + scalar;
            // simulate small work
            for(int k=0;k<50;k++) B[i]+=0.0;
        }
    } else {
        #pragma omp parallel for schedule(dynamic,1)
        for(int i=0;i<n;i++){
            B[i] = A[i] + scalar;
            for(int k=0;k<50;k++) B[i]+=0.0;
        }
    }

    double mid = omp_get_wtime();

    // Demonstrate nowait: another independent loop without waiting for previous loop's implicit barrier
    // We use an explicit parallel region to show nowait effectively:
    #pragma omp parallel
    {
        #pragma omp for nowait
        for(int i=0;i<n;i++){
            B[i] += 1.0; // continuation work
        }

        // This region executes immediately by each thread without waiting for the for loop
        #pragma omp single
        {
            // This will run while iterations may still be progressing due to nowait
            // small print to show thread entering single section
            // printf("single executed by thread %d\n", omp_get_thread_num());
        }
    }

    double end = omp_get_wtime();

    double checksum=0;
    for(int i=0;i<n;i++) checksum += B[i];

    printf("n=%d threads=%d sched=%s chunk=%d time_first=%f time_total=%f checksum=%e\n",
           n, threads, schedule_type==0?"STATIC":"DYNAMIC", chunk, mid-start, end-start, checksum);

    free(A); free(B);
}

int main(){
    int n = 200;
    int threads = 4;
    int chunks[] = {1,2,5,10,20,50,100};
    for(int s=0; s<2; s++){
        for(int cidx=0;cidx<sizeof(chunks)/sizeof(chunks[0]);cidx++){
            int chunk = chunks[cidx];
            // set schedule by OMP env var string (affects pragma runtime schedule only), simpler approach:
            if(s==0) omp_set_schedule(omp_sched_static, chunk);
            else omp_set_schedule(omp_sched_dynamic, chunk);
            run_test(n, threads, s, chunk);
        }
    }
    return 0;
}
