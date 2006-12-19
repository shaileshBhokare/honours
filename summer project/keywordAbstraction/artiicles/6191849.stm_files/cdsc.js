function random() {
   random.seed = (random.seed*random.a + random.c) % random.m;
   return random.seed / random.m;
}
function _rsEH(_rsE,_rsU,_rsL){}
function rsCi() {}
window.onerror=_rsEH;
random.m=714025;
random.a=4096;
random.c=150889;
random.seed = (new Date()).getTime()%random.m;
if (random() <= _rsSM) {
   var _rsRD=(new Date()).getTime();
   _rsIM='<scr'+'ipt language="JavaScript" type="text/javascript" src="'+_rsND+'cgi-bin/j?ci='+_rsCI+'&rd='+_rsRD+'&se=1&sv='+_rsSV+'"><\/scr'+'ipt>';
   document.write(_rsIM);
}
