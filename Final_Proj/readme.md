# CSCI 4060U Final Project Report

### For more detail please see

## Useful file structure

<pre><font color="#12488B"><b>.</b></font>
├── Report.md
├── Report_PDF.pdf
├── Report_PDF_Light.pdf
├── Report_PDF_V3.pdf
├── <font color="#12488B"><b>omp_ver</b></font>
│   ├── Build_In_Funcs.h
│   ├── Detector_pa.h
│   ├── <font color="#26A269"><b>omp2</b></font>
│   ├── <font color="#26A269"><b>omp4</b></font>
│   ├── <font color="#26A269"><b>omp6</b></font>
│   ├── script.cpp
├── <font color="#12488B"><b>omp_ver2</b></font>
│   ├── Build_In_Funcs.h
│   ├── Detector_pa.h
│   ├── <font color="#26A269"><b>omp</b></font>
│   ├── script.cpp
├── performance_data.txt
├── <font color="#12488B"><b>pics</b></font>
│   ├── <font color="#A347BA"><b>01-T.jpg</b></font>
│   ├── <font color="#A347BA"><b>01-dual.png</b></font>
│   ├── <font color="#A347BA"><b>02-T.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-T.jpg</b></font>
│   ├── <font color="#A347BA"><b>04-T.jpg</b></font>
│   ├── <font color="#A347BA"><b>05-T.png</b></font>
│   ├── <font color="#A347BA"><b>06-T.jpg</b></font>
│   ├── <font color="#A347BA"><b>06-T.png</b></font>
│   ├── <font color="#A347BA"><b>07-T.jpg</b></font>
│   ├── <font color="#A347BA"><b>07-T.png</b></font>
├── <font color="#12488B"><b>pthread_ver</b></font>
│   ├── Build_In_Funcs.h
│   ├── Detector_pthread.h
│   ├── <font color="#26A269"><b>a.out</b></font>
│   └── script.cpp
├── readme.md
├── <font color="#12488B"><b>seq_ver</b></font>
│   ├── Build_In_Funcs.h
│   ├── Detector_seq.h
│   ├── <font color="#26A269"><b>a.out</b></font>
│   ├── script.cpp
├── <font color="#12488B"><b>test_diff_img</b></font>
│   ├── <font color="#A347BA"><b>03-t-0-res.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-1-res.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-10-res.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-11-res.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-12-res.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-13-res.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-14-res.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-15-res.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-16-res.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-17-res.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-18-res.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-19-res.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-2-res.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-3-res.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-4-res.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-5-res.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-6-res.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-7-res.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-8-res.jpg</b></font>
│   └── <font color="#A347BA"><b>03-t-9-res.jpg</b></font>
├── <font color="#12488B"><b>test_img</b></font>
│   ├── <font color="#A347BA"><b>03-t-0-0.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-0-1.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-1-0.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-1-1.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-10-0.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-10-1.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-11-0.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-11-1.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-12-0.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-12-1.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-13-0.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-13-1.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-14-0.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-14-1.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-15-0.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-15-1.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-16-0.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-16-1.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-17-0.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-17-1.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-18-0.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-18-1.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-19-0.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-19-1.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-2-0.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-2-1.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-20-0.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-20-1.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-21-0.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-21-1.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-22-0.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-22-1.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-23-0.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-23-1.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-24-0.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-24-1.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-25-0.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-25-1.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-26-0.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-26-1.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-27-0.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-27-1.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-28-0.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-28-1.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-29-0.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-29-1.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-3-0.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-3-1.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-4-0.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-4-1.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-5-0.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-5-1.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-6-0.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-6-1.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-7-0.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-7-1.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-8-0.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-8-1.jpg</b></font>
│   ├── <font color="#A347BA"><b>03-t-9-0.jpg</b></font>
│   └── <font color="#A347BA"><b>03-t-9-1.jpg</b></font>
└── <font color="#12488B"><b>testimg</b></font>
    ├── <font color="#A347BA"><b>03-t-0-0.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-0-1.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-0.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-1-0.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-1-1.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-1.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-10-0.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-10-1.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-11-0.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-11-1.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-12-0.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-12-1.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-13-0.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-13-1.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-14-0.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-14-1.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-15-0.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-15-1.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-16-0.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-16-1.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-17-0.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-17-1.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-18-0.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-18-1.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-19-0.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-19-1.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-2-0.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-2-1.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-20-0.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-20-1.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-21-0.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-21-1.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-22-0.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-22-1.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-23-0.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-23-1.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-3-0.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-3-1.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-4-0.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-4-1.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-5-0.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-5-1.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-6-0.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-6-1.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-7-0.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-7-1.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-8-0.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-8-1.jpg</b></font>
    ├── <font color="#A347BA"><b>03-t-9-0.jpg</b></font>
    └── <font color="#A347BA"><b>03-t-9-1.jpg</b></font>
</pre>
