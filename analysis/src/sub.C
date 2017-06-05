{
    TCanvas canvas("sub","",700,900);
    canvas.Divide(2,2);

    TLatex text;
    text.SetNDC();
    TLine line;
    line.SetLineStyle(2);

    canvas.cd(1);

    hist_six_124->Draw();
    hist_six_blank_124->SetLineColor(4);
    hist_six_blank_124->Draw("same");
    hist_six_124_sub->SetLineColor(2);
    hist_six_124_sub->Draw("same");
    text.DrawLatex(.65,.75,"6m, ^{124}Sn");

    line.DrawLine(hist_six_124->GetBinCenter(tmin_6),0,
            hist_six_124->GetBinCenter(tmin_6),100);

    line.DrawLine(hist_six_124->GetBinCenter(tmax_6),0,
            hist_six_124->GetBinCenter(tmax_6),100);

    canvas.cd(2);

    hist_six_112->Draw();
    hist_six_blank_112->SetLineColor(4);
    hist_six_blank_112->Draw("same");
    hist_six_112_sub->SetLineColor(2);
    hist_six_112_sub->Draw("same");

    text.DrawLatex(.65,.75,"6m, ^{112}Sn");

    line.DrawLine(hist_six_124->GetBinCenter(tmin_6),0,
            hist_six_124->GetBinCenter(tmin_6),100);

    line.DrawLine(hist_six_124->GetBinCenter(tmax_6),0,
            hist_six_124->GetBinCenter(tmax_6),100);

    canvas.cd(3);

    hist_four_124->Draw();
    hist_four_blank_124->SetLineColor(4);
    hist_four_blank_124->Draw("same");
    hist_four_124_sub->SetLineColor(2);
    hist_four_124_sub->Draw("same");

    text.DrawLatex(.65,.75,"4m, ^{124}Sn");

    line.DrawLine(hist_four_124->GetBinCenter(tmin_4),0,
            hist_four_124->GetBinCenter(tmin_4),100);

    line.DrawLine(hist_four_124->GetBinCenter(tmax_4),0,
            hist_four_124->GetBinCenter(tmax_4),100);

    canvas.cd(4);

    hist_four_112->Draw();
    hist_four_blank_112->SetLineColor(4);
    hist_four_blank_112->Draw("same");
    hist_four_112_sub->SetLineColor(2);
    hist_four_112_sub->Draw("same");

    text.DrawLatex(.65,.75,"4m, ^{112}Sn");


    line.DrawLine(hist_four_124->GetBinCenter(tmin_4),0,
            hist_four_124->GetBinCenter(tmin_4),100);

    line.DrawLine(hist_four_124->GetBinCenter(tmax_4),0,
            hist_four_124->GetBinCenter(tmax_4),100);
}
