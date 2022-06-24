
<template>
 <div>
<div>
 <div class="head"><p style='margin-top:10px;margin-left:60px;height:20px;float:left;text-align:left;'>Online Object Detection</p></div>
<div class="gongneng">功能体验</div>
<div class="large">
    <div class="c1"><canvas id="chart" width="800px" height="700px" ref="previewImageCanvas" class="detect-image"></canvas></div>
    <div class="c2">

        <div class="input_control">
        <input type="text" class="form_input" placeholder="输入图片url地址" ref="imgurl"/>
           <!-- <img class="icon icon-search" src="../assets/sous.png" @click="testimg" > -->
        </div>
        <div>
              <div class="d3">或</div>
              <input class="d5" type="file" style="display:none" ref="uploadImage" @change="fileChange" accept="image/jpeg, image/png" multiple />
             <div class="d1"><button type="button" @click="dotest" class="button gray">上传图片</button></div>
             
             <div class="d2"><button type="button"  @click="exportCanvasAsPNG" class="button gray">结果下载</button></div>
            <div class="d4">图片格式：JPEG、JPG、PNG、BMP。 图片大小：小于3MB。</div>
        </div>
        </div>
    </div>
    <div class="c3"><ul><li v-for="box in detectResultJson" :key ="box.class_label">{{box.class_name}}  :  {{box.confidence}}</li></ul>
       <!-- <el-input type="textarea" :rows="32" placeholder="检测结果" v-model="detectResultJson" class="detect-result-json" :readonly="true">
                </el-input> -->
        


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
 .gongneng{
           position: absolute;
            top:100px;
            left:45%;

        font-size: xx-large;
        font-family: Arial, Helvetica, sans-serif;
        color:rgb(71, 78, 92);
 }
 .c1{
 position: relative;
 float:left;
            top:100px;
            left:10%;

 }
 .c2{
 background:#D7D7D7;
float:left;
 width: 45%;
  height: 100px;
  top:100px;
   position: relative;

            left:10%;
 }
 .c3{
      width: 45%;
      float:left;
  height: 20%;
  border:1px solid #c8cccf;
  position: relative;
left:10%;
  top:100px;
  font-size:16px;
            font-size: x-large;
            
            color:rgb(71, 78, 92);

 }
 .input_control{
  width:360px;
float:left;
  position: relative;
            top:20px;
            left:5%;
}
input[type="text"],#btn1,#btn2{
  box-sizing: border-box;

  font-size:16px;
  height:40px;
  width:80px;
  border-radius:4px;
  border:1px solid #c8cccf;
  color:#6a6f77;
  -web-kit-appearance:none;
  -moz-appearance: none;
  display:block;
  outline:0;
  padding:0 1em;
  text-decoration:none;
  width:100%;
}
input[type="text"]:focus{
  border:1px solid #ff7496;
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
background: -webkit-linear-gradient(top,#ddd, #bbb);
background: -moz-linear-gradient(top,#ddd, #bbb);
background: linear-gradient(top,#ddd, #bbb);
}
.d1{
float:left;
  position:  absolute;
            top:24px;
            left:60%;
}
.d5{

  position:  absolute;
  left:60%;
   top:24px;
  height:20px;
  width:160px;

}
.d2{
  position:  absolute;

            top:24px;
            left:80%;
}

.d3{
  position:  absolute;
            top:24px;
            left:50%;
}
.d4{
  position:  absolute;
            top:65px;
            left:6%;
            font-size: 15px;

        color:rgb(71, 78, 92);
}
</style>
<script>

export default {
    name: 'Books',
    data(){
        return {
            uploadFileSelectCallback: null,
            detectResultJson:[],
            aString:[""],
            class_label:"",
            class_name:"",
            confidence:"",
            imgname:"result",
            imgid:"chart",
            
        }
    },
    methods: {
//       
//canvas下载图片
    exportCanvasAsPNG:function() {
        var canvasElement = document.getElementById(this.imgid);
        var MIME_TYPE = "image/png";
        var imgURL = canvasElement.toDataURL(MIME_TYPE);
        var dlLink = document.createElement('a');
        dlLink.download = this.imgname;
        dlLink.href = imgURL;
        dlLink.dataset.downloadurl = [MIME_TYPE, dlLink.download, dlLink.href].join(':');
        document.body.appendChild(dlLink);
        dlLink.click();
        document.body.removeChild(dlLink);
    },


        fileChange: function(){
            let files = this.$refs.uploadImage.files;
            if(files.length != 1){
                this.$message.warning("请选择一个文件");
                return;
            }

            if(this.uploadFileSelectCallback)
                this.uploadFileSelectCallback(files[0]);

            this.$refs.uploadImage.value = null;
        },
        openFileDialog: function(callback){
            this.uploadFileSelectCallback = callback;
            this.$refs.uploadImage.click();
        },
        loadImage: function(url, callback){
            let image = new Image();
            image.onload = function(){
                callback(image);
            };
            image.src = url;
            // alert(image.src);
        },
        drawImageAndLabel: function(image, boxarray){

            this.detectResultJson = JSON.parse(JSON.stringify(boxarray, undefined, 4));
            let cvs = this.$refs.previewImageCanvas;
            let context = cvs.getContext("2d");
            let imageWidth = image.width;
            let imageHeight = image.height;
            let canWidth = cvs.width;
            let canHeight = cvs.height;
            let scale = Math.min(canWidth / imageWidth, canHeight / imageHeight);
            scale = Math.min(scale, 1);

            let dx = (canWidth - imageWidth * scale) / 2;
            let dy = (canHeight - imageHeight * scale) / 2;
            let dr = canWidth - dx;
            let db = canHeight - dy;
            context.clearRect(0, 0, canWidth, canHeight);
            context.drawImage(image, dx, dy, dr - dx + 1, db - dy + 1);

            if(boxarray == null || boxarray.length == 0)
                return;

            context.font = "26px 微软雅黑";
            context.save();
            context.translate(dx, dy);
            context.scale(scale, scale);

            var colors = ["#F0F", "#0FF", "#FF0", "#00F", "#0F0", "#F00", "#8F0", "#08F", "#0F8", "#88F", "#F80"];
            for(let i = 0; i < boxarray.length; ++i){
                let box = boxarray[i];
                let color = colors[box.class_label % colors.length];
                let label = box.class_name;
            

                context.beginPath();
                context.lineWidth = "5";
                context.strokeStyle = color;
                context.rect(
                    box.left, 
                    box.top, 
                    (box.right - box.left + 1), 
                    (box.bottom - box.top + 1));
                context.stroke();

                context.fillStyle = color;
                context.font = "60px 微软雅黑";
                context.fillText(label + " " + box.confidence.toFixed(2), box.left, box.top - 5);
            }
            context.restore();
        },
        showtext:function(boxarray){
              if(boxarray == null || boxarray.length == 0)
                return;
                
                  for(let i = 0; i < boxarray.length; ++i){
                     let box = boxarray[i];
                     this.aString[i]={
                         class_name:box.class_name,
                         class_label:box.class_label,
                         confidence:box.confidence,


                     }
                    
                    
                  }
              
              

        },
        dotest: function(){
            var _this = this;
            this.openFileDialog(function(file){
                var reader = new FileReader();
                reader.onload = function(e){ 
                    _this.loadImage(this.result, function(image){
                        
                        var token = ";base64,";
                        var base64_data = image.src;
                        var token_begin = base64_data.indexOf(token);
                        var clean_data = base64_data.substring(token_begin + token.length);
                        //   (clean_data);alert
                        Echo("/api/detectBase64Image")
                        .data(clean_data)
                        .then((boxarray)=>{
                            _this.drawImageAndLabel(image, boxarray);
                            _this.showtext(boxarray);
                        });
                    });
                };
              
                reader.readAsDataURL(file);
            })
        },
       getBase64(imgUrl, callback) {

      // 一定要设置为let，不然图片不显示
    //   alert(imgUrl);
      const image = new Image();

      // 解决跨域问题

      image.setAttribute("crossOrigin", "anonymous");

      const imageUrl = imgUrl;

      image.src = imageUrl;

      // image.onload为异步加载

      // eslint-disable-next-line no-undef

      image.onload = function() {

        var canvas = document.createElement("canvas");

        canvas.width = image.width;

        canvas.height = image.height;

        var context = canvas.getContext("2d");

        // eslint-disable-next-line no-undef

        context.drawImage(image, 0, 0, image.width, image.height);

        var quality = 0.8;

        // 这里的dataurl就是base64类型

        // 使用toDataUrl将图片转换成jpeg的格式,不要把图片压缩成png，因为压缩成png后base64的字符串可能比不转换前的长！

        const dataurl = canvas.toDataURL("image/jpeg", quality);

        callback ? callback(dataurl) : null; //调用回调函数

      };

    },


        testimg:function(){
            
         


    this.getBase64(this.$refs.imgurl.value, dataURL => {

                // alert(dataURL );

     });

            //  _this.loadImage(_this.$refs.imgurl.value, function(image){
                        
            //             var token = ";base64,";
            //             var base64_data = image.src;
            //             var token_begin = base64_data.indexOf(token);
            //             var clean_data = base64_data.substring(token_begin + token.length);
            //             Echo("/api/detectBase64Image")
            //             .data(clean_data)
            //             .then((boxarray)=>{
            //                 _this.drawImageAndLabel(image, boxarray);
            //                 _this.showtext(boxarray);
            //             });
            //         });
        },
        init: function(){
        }
    },
    mounted(){
        this.init()
    }
}


</script>