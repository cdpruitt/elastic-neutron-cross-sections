{
    int run_min = 59;
    int run_max = 115;

    double run[500];
    double zero[500];
    double mon[500];
    double bci[500];
    double zeroBci[500];
    double monBci[500];

    double one, two, three, four;

    ifstream file("record.dat");
    int Nrun = 0;
    double mean = 0.;
    double total = 0.;
    for (;;)
    {
        file >> one >> two >> three>> four;
        if (file.bad())break;
        if (file.eof())break;

        run[Nrun] = one;
        zero[Nrun] = two;
        mon[Nrun] = three;
        bci[Nrun] = four;
        monBci[Nrun] = three/four;
        zeroBci[Nrun] = two/four;
        mean += three/four;
        total += 1.;
        Nrun++;
    }

    TCanvas canvas("record","",800,700);
    canvas.Divide(1,2);
    canvas.cd(1);
    TH2S frame1("frame1","",10,run_min,run_max,10,0,.15);
    frame1.GetXaxis()->SetTitle("Run number");
    frame1.GetYaxis()->SetTitle("Zero/BCI");
    frame1.SetStats(kFALSE);
    frame1.Draw();
    TGraph gz(Nrun,run,zeroBci);
    gz.SetMarkerStyle(20);
    gz.Draw("P");
    canvas.cd(2);
    TH2S frame2("frame2","",10,run_min,run_max,10,0,.18);
    frame2.GetXaxis()->SetTitle("Run number");
    frame2.GetYaxis()->SetTitle("Ceil Mon/BCI");
    frame2->SetStats(kFALSE);
    frame2.Draw();
    TGraph gm(Nrun,run,monBci);
    gm.SetMarkerStyle(20);
    gm.Draw("P");

    mean = mean/total;
    cout << " mean= " << mean << endl;
    TLine line;
    line.SetLineStyle(2);
    line.DrawLine(run_min,mean,run_max,mean);
}
