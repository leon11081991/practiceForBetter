(function () {
    // 設定localStorage
    var initCustom = {  //用物件包資料
        'userName': 'user1234',
        'password': 'ok1234',
    }
    window.localStorage.setItem('customInfo', JSON.stringify(initCustom));  //先把資料物件轉為字串，再設置為localStorage


    var form = document.getElementById('login');
    var elements = form.elements;  //取得form裡面的表單元件
    // console.log(elements)
    var elMain = document.getElementById('main');
    var container = document.getElementById('container');
    // console.log(container);
    var elUsername = elements.username;  //表單元件裡，選到使用者名稱輸入框
    var elPwd = elements.pwd;  //表單元件裡，選到使用者密碼輸入框
    var elChk = elements.showPwd;  //表單元件裡，選到顯示密碼勾選框
    var elSubmit = elements.submit_btn;
    // var submitedStatus = false;  //表單的送出狀態

    elSubmit.disabled = true;  //初始submit不可按
    elSubmit.className = 'disabled';

    // -----未填寫完全，submit失效-----:
    form.addEventListener('input', function (e) {

        elSubmit.disabled = !(elPwd.value && elUsername.value);
        elSubmit.className = !(elPwd.value && elUsername.value) ? 'disabled' : 'enable';
    });

    // -----密碼顯示更改-----:
    elChk.addEventListener('change', function (e) { //(事件)在表單改變elChk做...
        try {
            if (elChk.checked) {
                elPwd.type = 'text';
            } else {
                elPwd.type = 'password';
            }
        } catch (error) {
            alert('發生錯誤，請重新再試一次。');
        }
    })

    // ----- Validate -----:

    jQuery.extend(jQuery.validator.messages, {
        required: "必選欄位",
        minlength: jQuery.validator.format("請輸入一個 長度最少是 {0} 的字串")
    });

    $().ready(function () {
        $('#login').validate({
            rules: {
                username: {
                    required: true,
                    minlength: 6
                },
                password: {
                    required: true,
                    minlength: 6
                }
            }
        })
    });



    // -----登入後顯示歡迎提示-----:
    form.addEventListener('submit', function (e) {  //(事件)在表單submit做...
        e.preventDefault();  //停止預設的發送行為
        var customInfo = JSON.parse(window.localStorage.getItem('customInfo'));  //將原本的localStorage解析為JSON

        // console.log('customInfo', typeof (customInfo));
        // console.log('customInfo', typeof (JSON.parse(customInfo)));
        console.log(customInfo.userName);
        console.log(customInfo.password);
        if (elUsername.value == customInfo.userName && elPwd.value == customInfo.password) {
            var msg = '登入成功! 哈囉! ' + elUsername.value;
            elMain.textContent = msg;  //覆寫掉原本裡面的內容，變成「登入成功」

            setTimeout(() => {  //設置歡迎語1.5秒後消失
                $('#login').css('display', 'none');
            }, 1000 * 1.5);

            container.style.display = 'block';
        } else {
            alert('帳號或密碼錯誤，請重新登入。(嘗試: user123/ok1234)');
        }

    })
})();  //IIFE結束//

$(function () {
    var searchTarget = {};

    var firstSelectInit = function (listData) {
        //console.log('listData',listData)  //listData是所有在陣列裡的資料
        var newList = [];  //空陣列要拿來放行政區的資料
        listData.forEach(dist => newList.push(dist.sarea))  //陣列裡的所有資料，把每一個(dist)放到空陣列裡
        var sareaList = [...new Set(newList)];  //用set()過濾掉重複的資料
        //console.log('sareaList',sareaList);

        sareaList.map(sarea => $('#district-sel').append($('<option></option>').val(sarea).text(sarea)))
        //把行政區列表陣列裡的行政區，映射到HTML的位置
    };

    var getSecondSelect = function (item) {
        // console.log('item:',item);
        // console.log('item.sno',item.sno);
        $.each(item, function (index, eachitem) {
            $('#station_sel').append($('<option></option>').attr({ 'placeId': index, 'lat': eachitem.lat, 'lng': eachitem.lng }).val(eachitem.sno).text(eachitem.sna))
        });
    };

    var searchBike = function (listData) {

        //console.log('listData', listData)
        //console.log('val', $("#station_sel").val())
        searchTarget = listData.filter(item => item.sno == $("#station_sel").val())[0];  //陣列裡的所有資料，過濾掉其他不要的資料，只留跟第二個選單一樣的資料
        // console.log('searchTarget', searchTarget);
        $('#result').css('display', 'block');  //按下搜尋按鍵，搜尋結果才會顯示出來
        $('#result').append($('.update-time').text('更新時間: ' + searchTarget.updateTime));
        $('#result').append($('.sta').text('您搜尋的站名: ' + searchTarget.sna));
        $('#result').append($('.number-now').text('場站目前車輛數量: ' + searchTarget.sbi));
        $('#result').append($('.empty-num').text('空位數量: ' + searchTarget.bemp));

    };


    $.ajax({  //document.ready後，接收第一次資料(為了做選單)
        url: 'https://tcgbusfs.blob.core.windows.net/dotapp/youbike/v2/youbike_immediate.json',
        method: 'GET',
        dataType: 'json',
        success: function (listData) {  //成功執行ajax做...
            firstSelectInit(listData);

            $('#district-sel').on('change', function () {  //當第一個選單被改變時做
                var district;
                var stationList;

                $('#station_sel').empty().append($('<option selected></option>').val('').text('----請選擇站名----'));
                district = $("#district-sel").val();
                // console.log($("#district-sel").val())
                stationList = listData.filter(item => item.sarea == district);  //只選出行政區跟第一選單一樣的資料
                getSecondSelect(stationList);


            });
        },
    }); // End First Time Ajax 

    $("#container").on('change', function () {  //驗證選單是否都有被填入
        // if($("#district-sel").val()&&$("#station_sel").val()){
        //     $("#startSearch").attr('disabled',false);
        // }else{
        //     $("#startSearch").attr('disabled',true);
        // }
        $('#result').css('display', 'none');  //改變選單，搜尋結果就會消失
        $("#startSearch").attr('disabled', !($("#district-sel").val() && $("#station_sel").val()));
        initMap();
    });

    $('#startSearch').on('click', function () {  //按下開始尋找後做...
        $.ajax({
            url: 'https://tcgbusfs.blob.core.windows.net/dotapp/youbike/v2/youbike_immediate.json',
            method: 'GET',
            dataType: 'json',
            success: function (listData) {
                // $('#result').empty();
                // console.log(listData);
                searchBike(listData);
                // console.log('lat', searchTarget.lat, 'lng', searchTarget.lng);
                putMarker(searchTarget);

                $("#startSearch").attr("disabled", "disabled");  // for prevent double send



            }
        })
    })
}); //End Document Ready


/////Google Map//////

var map;
function initMap() {  //初始化地圖
    map = new google.maps.Map(document.getElementById('map'), {  //生成一個google map物件
        center: { lat: 25.03746, lng: 121.564558 },
        zoom: 13
    });
}

function putMarker(searchTarget) {

    var marker = new google.maps.Marker({  //生成一個marker物件
        position: { lat: searchTarget.lat, lng: searchTarget.lng },
        map: map
    });
    // map.panTo(marker.getPosition());
    // console.warn(google.maps)
    map.setCenter(new google.maps.LatLng(searchTarget.lat, searchTarget.lng));   //google.maps => 物件
    map.setZoom(16);
}
