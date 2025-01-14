# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

# preamble...
proc on_reset {} {
    expect_pause
    continue
    glb_filter ON
    loc_filter SM_AO AO_Philo<2>
    current_obj SM_AO AO_Philo<2>
    current_obj TE l_philo<2>.m_timeEvt
}

# tests...
test "Philo-thinking tick"
tick
expect "           Tick<0>  Ctr=*"
expect "           TE0-ADis Obj=l_philo<2>.m_timeEvt,AO=AO_Philo<2>"
expect "@timestamp TE0-Post Obj=l_philo<2>.m_timeEvt,Sig=TIMEOUT_SIG,AO=AO_Philo<2>"
expect "@timestamp AO-Post  Sdr=QS_RX,Obj=AO_Philo<2>,Evt<Sig=TIMEOUT_SIG,*"
expect "@timestamp AO-GetL  Obj=AO_Philo<2>,Evt<Sig=TIMEOUT_SIG,*"
expect "@timestamp Disp===> Obj=AO_Philo<2>,Sig=TIMEOUT_SIG,State=thinking"
expect "@timestamp TE0-DisA Obj=l_philo<2>.m_timeEvt,AO=AO_Philo<2>"
expect "===RTC===> St-Exit  Obj=AO_Philo<2>,State=thinking"
expect "@timestamp QF-New   Sig=HUNGRY_SIG,*"
expect "@timestamp MP-Get   Obj=EvtPool1,*"
expect "@timestamp QF-gc    Evt<Sig=HUNGRY_SIG,*"
expect "@timestamp MP-Put   Obj=EvtPool1,*"
expect "===RTC===> St-Entry Obj=AO_Philo<2>,State=hungry"
expect "@timestamp ===>Tran Obj=AO_Philo<2>,Sig=TIMEOUT_SIG,State=thinking->hungry"
expect "@timestamp Trg-Done QS_RX_TICK"

test "Philo-hungry publish EAT" -noreset
publish EAT_SIG [binary format c 2]
expect "@timestamp QF-New   Sig=EAT_SIG,*"
expect "@timestamp MP-Get   Obj=EvtPool1,*"
expect "@timestamp QF-Pub   Sdr=QS_RX,Evt<Sig=EAT_SIG,*"
expect "@timestamp AO-Post  Sdr=QS_RX,Obj=AO_Philo<2>,Evt<Sig=EAT_SIG,*"
expect "@timestamp QUTEST_ON_POST EAT_SIG,Obj=AO_Philo<2> 2"
expect "@timestamp QF-gc?   Evt<Sig=EAT_SIG,*"
expect "@timestamp AO-GetL  Obj=AO_Philo<2>,Evt<Sig=EAT_SIG,*"
expect "@timestamp Disp===> Obj=AO_Philo<2>,Sig=EAT_SIG,State=hungry"
expect "@timestamp BSP_CALL BSP::random *"
expect "@timestamp TE0-Arm  Obj=l_philo<2>.m_timeEvt,AO=AO_Philo<2>,Tim=*,Int=0"
expect "===RTC===> St-Entry Obj=AO_Philo<2>,State=eating"
expect "@timestamp ===>Tran Obj=AO_Philo<2>,Sig=EAT_SIG,State=hungry->eating"
expect "@timestamp QF-gc    Evt<Sig=EAT_SIG,*"
expect "@timestamp MP-Put   Obj=EvtPool1,*"
expect "@timestamp Trg-Done QS_RX_EVENT"

test "Philo-eating tick" -noreset
tick
expect "           Tick<0>  Ctr=*"
expect "           TE0-ADis Obj=l_philo<2>.m_timeEvt,AO=AO_Philo<2>"
expect "@timestamp TE0-Post Obj=l_philo<2>.m_timeEvt,Sig=TIMEOUT_SIG,AO=AO_Philo<2>"
expect "@timestamp AO-Post  Sdr=QS_RX,Obj=AO_Philo<2>,Evt<Sig=TIMEOUT_SIG,Pool=0,Ref=0>,*"
expect "@timestamp AO-GetL  Obj=AO_Philo<2>,Evt<Sig=TIMEOUT_SIG,Pool=0,Ref=0>"
expect "@timestamp Disp===> Obj=AO_Philo<2>,Sig=TIMEOUT_SIG,State=eating"
expect "@timestamp QF-New   Sig=DONE_SIG,*"
expect "@timestamp MP-Get   Obj=EvtPool1,*"
expect "@timestamp QF-Pub   Sdr=AO_Philo<2>,Evt<Sig=DONE_SIG,Pool=1,Ref=0>"
expect "@timestamp QF-gc    Evt<Sig=DONE_SIG,Pool=1,Ref=1>"
expect "@timestamp MP-Put   Obj=EvtPool1,*"
expect "@timestamp TE0-DisA Obj=l_philo<2>.m_timeEvt,AO=AO_Philo<2>"
expect "===RTC===> St-Exit  Obj=AO_Philo<2>,State=eating"
expect "@timestamp BSP_CALL BSP::random *"
expect "@timestamp TE0-Arm  Obj=l_philo<2>.m_timeEvt,AO=AO_Philo<2>,Tim=*,Int=0"
expect "===RTC===> St-Entry Obj=AO_Philo<2>,State=thinking"
expect "@timestamp ===>Tran Obj=AO_Philo<2>,Sig=TIMEOUT_SIG,State=eating->thinking"
expect "@timestamp Trg-Done QS_RX_TICK"

test "Philo-thinking tick(2)" -noreset
tick
expect "           Tick<0>  Ctr=*"
expect "           TE0-ADis Obj=l_philo<2>.m_timeEvt,AO=AO_Philo<2>"
expect "@timestamp TE0-Post Obj=l_philo<2>.m_timeEvt,Sig=TIMEOUT_SIG,AO=AO_Philo<2>"
expect "@timestamp AO-Post  Sdr=QS_RX,Obj=AO_Philo<2>,Evt<Sig=TIMEOUT_SIG,*"
expect "@timestamp AO-GetL  Obj=AO_Philo<2>,Evt<Sig=TIMEOUT_SIG,*"
expect "@timestamp Disp===> Obj=AO_Philo<2>,Sig=TIMEOUT_SIG,State=thinking"
expect "@timestamp TE0-DisA Obj=l_philo<2>.m_timeEvt,AO=AO_Philo<2>"
expect "===RTC===> St-Exit  Obj=AO_Philo<2>,State=thinking"
expect "@timestamp QF-New   Sig=HUNGRY_SIG,*"
expect "@timestamp MP-Get   Obj=EvtPool1,*"
expect "@timestamp QF-gc    Evt<Sig=HUNGRY_SIG,*"
expect "@timestamp MP-Put   Obj=*"
expect "===RTC===> St-Entry Obj=AO_Philo<2>,State=hungry"
expect "@timestamp ===>Tran Obj=AO_Philo<2>,Sig=TIMEOUT_SIG,State=thinking->hungry"
expect "@timestamp Trg-Done QS_RX_TICK"

# the end
end
