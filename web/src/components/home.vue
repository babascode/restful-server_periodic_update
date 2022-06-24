<template>
 <div>
        <div>
            <div class="head"><p style='margin-top:10px;margin-left:60px;height:20px;float:left;text-align:left;'>Online Object Detection</p></div>
            <div class="min">
                <div class="a1">目标检测</div>
                <div class="a2">目标检测，也叫目标提取，是一种基于目标几何和统计特征的图像分割。它将目标的分割和识别合二为一，其准确性和实时性是整个系统的一项重要能力。</div>
                <div class="a3"><button type="button" class="button gray">立即体验</button></div>
            </div>
            <div id="max">
            <div class="re">
            <ul>
                <li><img src="../assets/1.png" alt="1"></li>
                <li><img src="../assets/2.png" alt="2"></li>
                <li><img src="../assets/3.png" alt="3"></li>

            </ul>
            <ol>
                <li></li>
                <li></li>
                <li></li>

            </ol>
            </div>
            </div>


        </div>
        <div class="123">
        <div class="zhuyao"> 主要功能</div>
        <hr  width="100%" size="1" color="white">
        </div>
        <div class="e">
    <router-link to="/books"><div class="div-one">

                <div class="div-two">
                    <img src="../assets/flower.jpeg" style="width:280px;height:190px">

                </div>
                <div class="b1">图片检测</div>
                <div class="b2">检测图像中的物体。可识别80类物体，例如：人体、自行车、苹果、三明治、旗帜、大象等。</div>
            </div></router-link>
 <div class="div-one">

                <div class="div-two">
                    <img src="../assets/gong.jpeg" style="width:280px;height:190px">

                </div>
                <div class="b1">着装检测</div>
      <div class="b2">检测指定场景范围内的人员穿戴情况（头盔、口罩、工作服等），不规范穿戴行为实时预警。</div>
            </div>
 <router-link to="/uploadvideo"><div class="div-one">

                <div class="div-two">
                    <img src="../assets/video.jpeg" style="width:280px;height:190px">

                </div>
                <div class="b1">视频检测</div>
      <div class="b2">可通过输入的短视频（如10s）检测视频中是否存在人体、车辆、宠物等。</div>
            </div></router-link>
 <div class="div-one">

                <div class="div-two">
                    <img src="../assets/22.png" style="width:280px;height:190px">

                </div>
                <div class="b1">物体定位</div>
      <div class="b2">检测图像中的所有物体，返回该物体的区域位置/坐标信息。</div>
            </div>
            </div>
    </div>
</template>
<style scoped>
.head{
        font-size: xx-large;
        font-family: Arial, Helvetica, sans-serif;
        color:white;
        top:35px;
        left:40px;

        height:80px;
        background-color: rgb(71, 78, 92);
    }


    #max{
  width: 700px;
  height: 340px;
display:inline-block;
position: relative;
   left: 300px;


}
.re{
  position: relative;
  height: 400px;
}
.re ul{
  list-style-type:none ;
}
.re ul>li{
  width: 600px;
  height: 300px;
  position: absolute;

  transition: 1s;
  opacity: 0;
}
.re ul>li img{
  width: 900px;
  height: 500px;

}
#max ol {
  position: relative;
  display: grid;
  grid-template-columns: repeat(5,75px);
  grid-template-rows: auto;
  grid-gap: 1em;gap: 1em;
  float: right;
  margin-top: 450px;
  list-style: none;
  top:0;left:0;
}
#max ol li {
  width: 25px;
  height: 10px;
  font-size: 15px;
  line-height: 30px;
  float: left;
  text-align: center;
  border-radius: 2em;
  border: 3px solid #999999;
}
.min {
position: relative;
float:left;

 width: 500px;
  height: 500px;
}
.a1{
          position: absolute;
            top:100px;
            left:300px;
        font-size: xx-large;
        font-family: Arial, Helvetica, sans-serif;
        color:rgb(71, 78, 92);
}
.a2{
       position: absolute;
       top:200px;
       left:300px;
       width: 500px;

        font-size: large;
        font-family: Arial, Helvetica, sans-serif;
        color:rgb(71, 78, 92);
}
.a3{
      position: absolute;
       top:450px;
       left:300px;

}
  .button{
width: 140px;
line-height: 38px;
text-align: center;
font-weight: bold;
color: yellow;
text-shadow:1px 1px 1px #333;
border-radius: 5px;
margin:0 20px 20px 0;
position: relative;
overflow: hidden;
}
.button.gray{
color: rgb(71, 78, 92);
text-shadow:1px 1px 1px #fff;
border:1px solid #dce1e6;
box-shadow: 0 1px 2px #fff inset,0 -1px 0 #a8abae inset;
background: -webkit-linear-gradient(top,#f2f3f7,#e4e8ec);
background: -moz-linear-gradient(top,#f2f3f7,#e4e8ec);
background: linear-gradient(top,#f2f3f7,#e4e8ec);
}
	.div-one{
            width: 280px;
            height: 320px;
            position: relative;
           border:1px solid #D7D7D7;
            text-align:center;
       
            top: 230px;
            bottom: 0;
            display:inline-block;
            margin-left:40px;
                 }
        .div-two{
            position: absolute;
            left: 0;
            right: 0;
            top: 0;
            bottom: 0;


            ;
        }
        .b1{
            position: absolute;
            top:200px;
            left:30px;
            font-size:14px;
            font-size: large;
            font-family: Arial, Helvetica, sans-serif;
            color:rgb(71, 78, 92);
        }
        .b2{
            position: absolute;
            top:230px;
            left:25px;
text-align:left;
            font-size:15px;
            font-family: Arial, Helvetica, sans-serif;
            color:grey;
        }
        .zhuyao{

            height:20px;
            position:absolute ;
            font-size: xx-large;
            font-family: Arial, Helvetica, sans-serif;
            color:rgb(71, 78, 92);

            left: 45%;

            top: 620px;
            bottom:10px;
            display:inline-block;
        }
         .e{
        position:absolute ;
         left: 265px;

            top: 450px;
         
        }


</style>
<script>
window.onload = function(){
  var box=this.document.getElementsByClassName("re")[0];
  var lik=box.getElementsByTagName("li");
  function fun(i,j){//转换图片函数，就是把透明度改了一下
    lik[i].style.opacity=1;
    lik[j].style.opacity=0;
    lik[i+3].style.backgroundColor="#ffffff";//改一下小图标
    lik[j+3].style.backgroundColor="#00000000"
  }
  fun(0,1);//初始化下
  var i =0;
  function auto(){//轮播循环函数
    if(++i>=3){
      i=0;
      fun(0,2);
    }
    else fun(i,i-1);
  }
  timer=this.setInterval(auto,2000);
  box.onmouseover = function () { //鼠标划上去，停止轮播
    console.log('good');
    clearInterval(timer);
  }
  box.onmouseout = function () { //鼠标划出，继续轮播
    timer = setInterval(auto, 2000); //调用定时器
  }
  var j =0;
  for(;j<3;j++){//点击小图标也可以转换图片
    lik[j+3].ind=j;
    lik[j+3].onclick=function(){
      fun(this.ind,i)
      i=this.ind;
    }
  }

}
export default {
    name: 'Home',
    
}
</script>