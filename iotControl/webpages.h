//----------------------這裡是主頁面 (/)--------------------
static const char mainPage[] PROGMEM = u8R"(
<!DOCTYPE html>
<html>
<head>
<meta charset='utf-8'>
<title>WebServer</title>
</head>
<body>
<form action='/' method='get'>
  <table align='center'>
  <tr><td>設備位置</td><td><input id='ipaddr' type='text' readonly ></td></tr>
  <tr><td>位置選擇</td><td><select id='pin'><option value='16' selected>房間</option><option value='5'>書房</option><option value='4'>客廳</option><option value='0'>飯廳</option></select></td></tr>
  <tr><td>開關選擇</td><td><select id='sw'><option value='on' selected>開燈</option><option value='off'>關燈</option><option value='toggle'>換燈</option></select></td></tr> 
  <tr><td>點燈回應</td><td><input id='led' type='text' size='15' value='On/Off' readonly ></td><td><input type='button' value='點燈' onclick=sendControl('led')></td></tr>
  <tr><td>類比偵測</td><td><input id='vr' type='text' size='15' value='0~1023' readonly ></td><td><input type='button' value='取值' onclick=getSensor('vt')></td></tr> 
  <tr><td>數位偵測</td><td><input id='bt' type='text' size='15' value='0~1' readonly ></td><td><input  type='button' value='狀態' onclick=getSensor('bt')></td></tr>
 </table>
</form>
<script>
   document.getElementById('ipaddr').value=location.href; 
   function sendControl(id)
   { var ipaddr=document.getElementById('ipaddr');
     var pin=document.getElementById('pin');
	 var sw=document.getElementById('sw');
	 var url='/led?id='+pin.options[pin.selectedIndex].value+'&sw='+sw.options[sw.selectedIndex].value;
	 fetch(url)
     .then(res => res.text())
     .then(data => document.getElementById(id).value=data)
     .catch(err => alert(err));
   }   
   function getSensor(id)
   {
	  fetch('\sensor?id='+id)
     .then(res => res.text())
     .then(data => document.getElementById(id).value=data)
     .catch(err => alert(err));
   }
</script>
</boby>
</html>
)";

//----------------------這裡是錯誤路徑頁面--------------------
static const char errorPage[] PROGMEM= u8R"(
<!DOCTYPE html>
<html>
<body>  
  <H1>Error?</H1>
</body>
</html>
)";

//---------------------這裡是設定頁面 (/setting)  
static const char settingPage[] PROGMEM= u8R"(
<!DOCTYPE html>
<html>
<body>  
  <H1>Setting....</H1>
</body>
</html>
)";
//---------以下這一行不要刪除, 這可以讓網頁資料儲存在程式區, 節省記憶體--------------
#define WEBPAGE_IN_PROGMEM