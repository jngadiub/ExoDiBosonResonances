void make_latex_table_SigEff(){

  ifstream inf("efficiencies_MCSig.txt",ios::in);

  float m, e1jhp,m1jhp,e1jlp,m1jlp,e2j,m2j;
  char ch_m[32],ch_e1jhp[32],ch_m1jhp[32],ch_e1jlp[32],ch_m1jlp[32],ch_e2j[32],ch_m2j[32];
  while(inf.good()){
    inf>>m>>e1jhp>>m1jhp>>e1jlp>>m1jlp>>e2j>>m2j;
    sprintf(ch_m,"%.0f",m);      
    sprintf(ch_e1jhp,"%.3f",e1jhp);      
    sprintf(ch_e1jlp,"%.3f",e1jlp);      
    sprintf(ch_e2j,"%.3f",e2j);      
    sprintf(ch_m1jhp,"%.3f",m1jhp);      
    sprintf(ch_m1jlp,"%.3f",m1jlp);      
    sprintf(ch_m2j,"%.3f",m2j);      
      
    cout<<ch_m<<"\t &"<<ch_e1jhp<<"\t \\T&"<<ch_e1jlp<<"\t \\T&"<<ch_e2j<<"\t &"<<ch_m1jhp<<"\t \\T&"<<ch_m1jlp<<"\t \\T&"<<ch_m2j<<"\\\\"<<endl;
  }

}
