# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

# preamble...
proc on_reset {} {
    expect_pause
    continue
    glb_filter ON
    loc_filter SM_AO AO_Table
    current_obj SM_AO AO_Table
}

# tests...
test "post HUNGRY"
post HUNGRY_SIG [binary format c 2]
expect "@timestamp QF-New   Sig=HUNGRY_SIG,*"
expect "@timestamp MP-Get   Obj=EvtPool1,*"
expect "@timestamp AO-Post  Sdr=QS_RX,Obj=AO_Table,Evt<Sig=HUNGRY_SIG,*"
expect "@timestamp QUTEST_ON_POST HUNGRY_SIG,Obj=AO_Table 2"
expect "@timestamp AO-GetL  Obj=AO_Table,Evt<Sig=HUNGRY_SIG,*"
expect "@timestamp Disp===> Obj=AO_Table,Sig=HUNGRY_SIG,State=serving"
expect "@timestamp BSP_CALL BSP::displayPhilStat 2 hungry  "
expect "@timestamp QF-New   Sig=EAT_SIG,*"
expect "@timestamp MP-Get   Obj=EvtPool1,*"
expect "@timestamp QF-Pub   Sdr=AO_Table,Evt<Sig=EAT_SIG,*"
expect "@timestamp QF-gcA   Evt<Sig=EAT_SIG,Pool=1,Ref=2>"
expect "@timestamp QF-gcA   Evt<Sig=EAT_SIG,Pool=1,Ref=2>"
expect "@timestamp QF-gcA   Evt<Sig=EAT_SIG,Pool=1,Ref=2>"
expect "@timestamp QF-gcA   Evt<Sig=EAT_SIG,Pool=1,Ref=2>"
expect "@timestamp QF-gcA   Evt<Sig=EAT_SIG,Pool=1,Ref=2>"
expect "@timestamp QF-gc    Evt<Sig=EAT_SIG,Pool=1,Ref=1>"
expect "@timestamp MP-Put   Obj=EvtPool1,*"
expect "@timestamp BSP_CALL BSP::displayPhilStat 2 eating  "
expect "@timestamp =>Intern Obj=AO_Table,Sig=HUNGRY_SIG,State=serving"
expect "@timestamp QF-gc    Evt<Sig=HUNGRY_SIG,*"
expect "@timestamp MP-Put   Obj=EvtPool1,*"
expect "@timestamp Trg-Done QS_RX_EVENT"

test "post HUNGRY(2)" -noreset
post HUNGRY_SIG [binary format c 1]
expect "@timestamp QF-New   Sig=HUNGRY_SIG,*"
expect "@timestamp MP-Get   Obj=EvtPool1,*"
expect "@timestamp AO-Post  Sdr=QS_RX,Obj=AO_Table,Evt<Sig=HUNGRY_SIG,*"
expect "@timestamp QUTEST_ON_POST HUNGRY_SIG,Obj=AO_Table 1"
expect "@timestamp AO-GetL  Obj=AO_Table,Evt<Sig=HUNGRY_SIG,*"
expect "@timestamp Disp===> Obj=AO_Table,Sig=HUNGRY_SIG,State=serving"
expect "@timestamp BSP_CALL BSP::displayPhilStat 1 hungry  "
expect "@timestamp =>Intern Obj=AO_Table,Sig=HUNGRY_SIG,State=serving"
expect "@timestamp QF-gc    Evt<Sig=HUNGRY_SIG,*"
expect "@timestamp MP-Put   Obj=EvtPool1,*"
expect "@timestamp Trg-Done QS_RX_EVENT"

test "post DONE" -noreset
publish DONE_SIG [binary format c 2]
expect "@timestamp QF-New   Sig=DONE_SIG,*"
expect "@timestamp MP-Get   Obj=EvtPool1,*"
expect "@timestamp QF-Pub   Sdr=QS_RX,Evt<Sig=DONE_SIG,*"
expect "@timestamp AO-Post  Sdr=QS_RX,Obj=AO_Table,Evt<Sig=DONE_SIG,*"
expect "@timestamp QUTEST_ON_POST DONE_SIG,Obj=AO_Table 2"
expect "@timestamp QF-gcA   Evt<Sig=DONE_SIG,Pool=1,Ref=2>"
expect "@timestamp AO-GetL  Obj=AO_Table,Evt<Sig=DONE_SIG,*"
expect "@timestamp Disp===> Obj=AO_Table,Sig=DONE_SIG,State=serving"
expect "@timestamp BSP_CALL BSP::displayPhilStat 2 thinking"
expect "@timestamp QF-New   Sig=EAT_SIG,*"
expect "@timestamp MP-Get   Obj=EvtPool1,*"
expect "@timestamp QF-Pub   Sdr=AO_Table,Evt<Sig=EAT_SIG,Pool=1,Ref=0>"
expect "@timestamp QF-gcA   Evt<Sig=EAT_SIG,Pool=1,Ref=2>"
expect "@timestamp QF-gcA   Evt<Sig=EAT_SIG,Pool=1,Ref=2>"
expect "@timestamp QF-gcA   Evt<Sig=EAT_SIG,Pool=1,Ref=2>"
expect "@timestamp QF-gcA   Evt<Sig=EAT_SIG,Pool=1,Ref=2>"
expect "@timestamp QF-gcA   Evt<Sig=EAT_SIG,Pool=1,Ref=2>"
expect "@timestamp QF-gc    Evt<Sig=EAT_SIG,Pool=1,Ref=1>"
expect "@timestamp MP-Put   Obj=EvtPool1,*"
expect "@timestamp BSP_CALL BSP::displayPhilStat 1 eating  "
expect "@timestamp =>Intern Obj=AO_Table,Sig=DONE_SIG,State=serving"
expect "@timestamp QF-gc    Evt<Sig=DONE_SIG,*"
expect "@timestamp MP-Put   Obj=EvtPool1,*"
expect "@timestamp Trg-Done QS_RX_EVENT"


# the end
end
