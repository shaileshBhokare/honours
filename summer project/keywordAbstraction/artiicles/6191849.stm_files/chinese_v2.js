/*Generic pop up code - please use this only unless you really need something different*/
function popUpPage(url, parameters, name)
{
	var day = new Date();
	var pageName = name ? name : day.getTime()

	eval("bbc"+pageName+" = window.open('"+url+"','"+pageName+"','"+parameters+"')");

	if (eval("bbc"+pageName) && window.focus) eval("bbc"+pageName).focus();
}


/* Media Selector pop up code */

function popwin(aPage, aTarget, w, h, var1, var2){
	        window.open (aPage,aTarget,'status=no,scrollbars=no,resizable=yes,width='+w+',height='+h);

}


/* launch code for avconsole */
function launch_main_player(site)
{
	
	if (site == null) //no site name passed in - have to leave this check in
	{
		clickmain=window.open("http://news.bbc.co.uk/broadband/news_console.stm","clickmain","toolbar=0,location=0,status=0,menubar=0,scrollbars=0,resizable=0,top=100,left=100,width=671,height=373");
	}
	else
	{
		if (site == 'ukfs')
		{
			clickmain=window.open("http://news.bbc.co.uk/broadband/news_console.stm","clickmain","toolbar=0,location=0,status=0,menubar=0,scrollbars=0,resizable=0,top=100,left=100,width=671,height=373");

		}
		else if (site == 'ifs')
		{
			clickmain=window.open("/narrowband/static/audio_video/avconsole/ifs/f_news_console.stm","clickmain","toolbar=0,location=0,status=0,menubar=0,scrollbars=0,resizable=0,top=100,left=100,width=671,height=373");
		}
	}
}

function getPlatform()
{
	var myUserAgent;
	myUserAgent = navigator.userAgent.toLowerCase();

	if ((myUserAgent.indexOf("win") != -1) ||  (myUserAgent.indexOf("16bit") != -1))
	{
		return "win";
	}
	
	if (myUserAgent.indexOf("mac") != -1)
	{
		return "mac";
	}  
	
	if (myUserAgent.indexOf("x11") != -1)
	{
		return "unx";
	}  
	
	return "other";
}

function getBrowserType()
{
	var myUserAgent;

	var myMajor;
	myUserAgent= navigator.userAgent.toLowerCase();
	myMajor= parseInt(navigator.appVersion);
	if( (myUserAgent.indexOf('mozilla')!= -1) &&(myUserAgent.indexOf('spoofer')== -1) &&(myUserAgent.indexOf('compatible') == -1) &&(myUserAgent.indexOf('opera') == -1) &&(myUserAgent.indexOf('webtv')  == -1) )
	{  
		if (myMajor > 3)
		{
			return "nav4";
		}
	
		return "nav";
	}
	
	if (myUserAgent.indexOf("msie")!= -1)
	{  
	
		if (myMajor > 3)
		{
			return "ie4";
  		} 
	
		return "ie";
	}
// dom compliant browsers are allowed
	if(document.body.firstChild) return "domCompliant";
	return "other";
}

function getBrowserVersion() //this is for the ticker to identify between mac IE4 and IE5
{
	var s = navigator.appVersion;
	s = s.substr(s.indexOf("("),s.length);
	while (isNaN(parseInt(s)) && (s.length > 0))
	{
		alert('s - '+ s);
		s = s.substr(1,s.length);
	}
	return parseInt(s);
}

function request_launch(site)
{
	if (getPlatform() != "other" &&(getBrowserType() == "ie4" || getBrowserType() == "nav4" || getBrowserType() == "domCompliant"))
	{
		launch_main_player(site);
	} 
	else 
	{
		self.location.href="/1/shared/bsp/hi/services/help/html/av_console_browsers.stm";
	}
	
	return;
}


function openWindow(){
	var mywin = null;var unLoad;
	var surl = "http://news.bbc.co.uk/hi/english/static/business/data_desktop/mardata/ftse.stm";
	if (!mywin){mywin = window.open(surl,'BBCNewsOnline','toolbar=0,location=0,directories=0,status=0,menubar=0,scrollbars=0,resizable=0,width=370,height=292');
		mywin.location = surl;
		if (mywin.opener == null) mywin.opener = window; 
		mywin.opener.name = "opener";
	}else{
		if (mywin.closed){	
			mywin = null;openWindow();
		}
		if (mywin.focus) mywin.focus();
		mywin.location.href = surl;
	}
}



function popup(url) {
	day = new Date();
	id = day.getTime();
	eval("page" + id + " = window.open(url, '" + id + "', 'toolbar=0,location=0,directories=0,status=0,menubar=0,scrollbars=0,resizable=0,width=370,height=220');");
} 


function popUp(pageurl,width,height,scroll){
	day = new Date();
	id = day.getTime();
	if (window.screen) {  
		lpos = (screen.width/2)-(width/2);  
		hpos = (screen.height/2)-(height/2);
	}else	{
		lpos = 1;
		hpos = 1;		
	}		 		
	eval("bbcnews"+id+" = window.open('"+pageurl+"','"+id+"','toolbar=0,scrollbars="+scroll+",location=0,status=0,menubar=0,resizable=0,width="+width+",height="+height+",left="+lpos+",top="+hpos+"')");
}

function aodpopup(URL){
var high=330;
ismac=false;
if(navigator.userAgent.indexOf("Mac")!=-1){
	high=435;
}
//add 70px to height if mac
window.open(URL,'aod','width=662,height='+high+',top=0,toolbar=no,personalbar=no,location=no,directories=no,statusbar=no,menubar=no,status=no,resizable=yes,left=60,screenX=60,top=100,screenY=100');
}
window.name="main";

if(location.search.substring(1)=="focuswin"){
	window.focus();
}

function setHome(ssit_arg){
if(navigator.appVersion.charAt(navigator.appVersion.indexOf("MSIE") + 5) >= 5 && navigator.platform.indexOf("Win16") == -1 
&& navigator.platform.indexOf("Mac") == -1){
ssit_arg.style.behavior='url(#default#homepage)';
ssit_arg.setHomePage(document.location);
ssit_arg.href="#";
return true;
}
else {return false}
}

//RSS
function getArgs() { 
	var Args = new Object(); 
	var query = location.search.substring(1); 
	var pairs = query.split("&"); 
	for (var i = 0; i < pairs.length; i++) 
	{ 
		var pos = pairs[i].indexOf('='); 
		if (pos == -1) continue;
		var argname = pairs[i].substring(0,pos); 
		var value = pairs[i].substring(pos+1); 
		Args[argname] = unescape(value); 
	} 
	return Args;
}

function getRssUrl() { 
if (document.getElementsByTagName)
{
	var url; // = "http://news.bbc.co.uk/chinese/simp/hi/institutional/rss/";
	var linkTags = document.getElementsByTagName("link");
	var rssURI;
	var text;
	
	var pattern = /simp/;
	if (pattern.exec(document.location.href))
	  {
	    url = "http://news.bbc.co.uk/chinese/simp/hi/institutional/rss/";
	    text = "RSS 是什么？"; 
	  }
	else 
	  {
		url = "http://news.bbc.co.uk/chinese/trad/hi/institutional/rss/";
	    text = "RSS琌ぐ&#40637; ";
	  }

	for (var i = 0; i < linkTags.length; i++) {if (linkTags[i].getAttribute('type') == "application/rss+xml") {rssURI = linkTags[i].getAttribute('href');}}
	if (rssURI)
	  {
	    document.write('<div class="feedslink"><a href="'+rssURI+'"><img height="16" hspace="0" vspace="0" border="0" width="16" alt="News feeds" src="http://newsimg.bbc.co.uk/shared/img/v3/feed.gif" title="News feeds" align="right" /></a><a href="'+url+'" title="">'+text+'</a><span class="feedbar">&nbsp;|&nbsp;</span></div>')
       }
}
}

function getRssUrlStory(rssURI) { 

if (document.getElementsByTagName)
{
	var url; // = "http://news.bbc.co.uk/chinese/simp/hi/institutional/rss/";
    var text;
	
	

	if (rssURI)
	  {
	    var pattern = /simp/;
	    if (pattern.exec(document.location.href))
	  {
	    //url = "http://news.bbc.co.uk/chinese/simp/hi/institutional/rss/";
		//text = "RSS 是什么？";
		
		document.write('<span class="feedslink"><a href="'+rssURI+'"><img height="16" hspace="7" vspace="0" border="0" width="16" alt="News feeds" src="http://newsimg.bbc.co.uk/shared/img/v3/feed.gif" title="News feeds" align="left" /></a> <span class="feedbar">|</span> <a href="http://news.bbc.co.uk/chinese/trad/hi/institutional/rss/default.stm?rss='+rssURI+'" title="">RSS 是什么？</a></span><br clear="all"/>')
		
		
	  }
	else 
	  {
		//url = "http://news.bbc.co.uk/chinese/trad/hi/institutional/rss/";
		//text = "RSS琌ぐ?";
		document.write('<span class="feedslink"><a href="'+rssURI+'"><img height="16" hspace="7" vspace="0" border="0" width="16" alt="News feeds" src="http://newsimg.bbc.co.uk/shared/img/v3/feed.gif" title="News feeds" align="left" /></a> <span class="feedbar">|</span> <a href="http://news.bbc.co.uk/chinese/trad/hi/institutional/rss/default.stm?rss='+rssURI+'" title="">RSS琌ぐ&#40637; </a></span><br clear="all"/>')
	  }
	  
	  
	  
	  
	  
	    
      }
}
}

// pic Gallery colours
var currentPicColor = '#6A6dbe';
var unselectedColor = '#009';
var deadLinkColor = '#ccc';

function toggleCollapsable(linkId, contentId) {
	
	if (document.getElementById(contentId).style.display == "none") {
		
		for (var i = 2; i < arguments.length; i+=2) {	
			document.getElementById(arguments[i]).innerHTML = collapsableShowText ;
			document.getElementById(arguments[i]).className = 'show'; 
			document.getElementById(arguments[i+1]).style.display = "none";
		}
		document.getElementById(linkId).innerHTML = collapsableHideText ;
		document.getElementById(linkId).className = 'hide'; 
		document.getElementById(contentId).style.display = "";
	} else {
		document.getElementById(linkId).innerHTML = collapsableShowText;
		document.getElementById(linkId).className = 'show'; 
		document.getElementById(contentId).style.display = "none";
	}
}
