<template>
<div>
 <div class="head"><p style='margin-top:10px;margin-left:60px;height:20px;float:left;text-align:left;'>Online Object Detection</p></div>
<div class="gongneng">功能体验</div>
<div class="test1"> 
  <div class="test2">
    <el-upload style="margin-left:14%;margin-top:8%;"
       class="upload-demo"
      :drag="{Plus}"
      action="/api/detectVideo"
      multiple
      :show-file-list="false"
      
      :on-success="handleVideoSuccess"
      :before-upload="beforeUploadVideo"
      :on-progress="uploadVideoProcess">
           <i v-if="Plus" class="el-icon-upload"></i>
      <div v-if="Plus" class="el-upload__text" style="font-size:large;">将文件拖到此处，或<em>点击上传</em></div>
      <el-progress v-if="videoFlag == true" type="circle" :percentage="videoUploadPercent" style="margin-top:30px;"></el-progress>
      <div class="el-upload__tip" slot="tip" style="font-size: large;">只能上传mp4/flv/avi文件，且不超过300M</div>

      <!-- <i v-if="Plus" class="el-icon-upload"></i>
      <div v-if="Plus" class="el-upload__text" style="width:400px;font-size:18px;">将文件拖到此处，或<em>点击上传</em></div>
      <el-progress v-if="videoFlag == true" type="circle" :percentage="videoUploadPercent" style=""></el-progress>
      <div class="el-upload__tip" style="width:400px;font-size:14px;" slot="tip">只能上传mp4/flv/avi文件，且不超过300M</div> -->
    </el-upload>
         <div class="test3">  <input  class="button" type="button" value="显示结果" @click="change" style="margin-top:30px;"></div>
  
  </div>

     <div  class="video"  > 
      


       <video-player  v-show="isShow"
        class="video-player vjs-custom-skin"
        style="margin-top:-30px"
        id="videoPlayer"
       
        :options="playerOptions"
      >
     

       </video-player>
     </div>
</div>
</div>
</template>
<style>

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
.video{
  left:40%;
   top:10%;
  width: 45%;
  top:90px;
 position: absolute;
}
.button{
  background-color: #2ea0e2; /* Green */
    border: none;
    color: white;
    padding: 15px 32px;
    text-align: center;
    text-decoration: none;
    display: inline-block;
    font-size: 16px;

}
.test3{


    position: absolute;
    left:25%;
   top:350px;
 
}

.test2{
  width:800px;

  top:20%;
 float: left;
  position: absolute;
}
.test1{

   font-size: xx-large;
   position: relative;
   margin-top:5%;
   margin-left:5%;
   
}
</style>
<script>


export default {
  
  name: 'uploadvideo',
  data () {
    return {
      videoForm: {
        videoId: '',
        videoUrl: ''
      },
      isShow: false,
      videoFlag: false,
      Plus: true,
      viurl:'',
      videoUploadPercent: 0,
      //videoSrc:'http://127.0.0.1:8090/static/re_video.mp4',
      playerOptions: {
       playbackRates: [0.5, 1.0, 1.5, 2.0],//倍速控制
       autoplay: false,//是否自动播放
       muted: false,//是否静音播放
       loop: false,//是否循环播放
       preload: "auto",
       language: "zh-CN", //语言，还要引入对应的文件
       aspectRatio: "16:9",//比例
       fluid: true,
       sources: [
         {
           type: "video/mp4",
          src: require( '/home/xd2/hhx2022/projects/trt/tensorrt-integrate/restful-server/workspace/re_video.mp4'),
          // src: "http://127.0.0.1:8090/static/re_video.mp4"
          
         },
       ],
      //poster: require("../assets/upvideo.png"), //你的封面地址
       notSupportedMessage: "此视频暂无法播放，请稍后再试",
       controlBar: {
         timeDivider: true,
         durationDisplay: true,//剩余时间是否显示
         remainingTimeDisplay: false,//剩余时间是否显示，有一个即可
         fullscreenToggle: true,//全屏按钮
       },
     },
    }
  },
  
 
  methods: {
 
    playVideo(){



    },
                    change:function(){

                    this.isShow = !this.isShow; 
                
    //                let vdo = document.getElementById("videoPlayer")
    //   document.getElementById("videoPlayer").load();
    //  document.getElementById("videoPlayer").load();
  

                },
   // 视频上传前执行
    beforeUploadVideo (file) {
     
      const isLt300M = file.size / 1024 / 1024 < 300
      if (['video/mp4', 'video/ogg', 'video/flv', 'video/avi', 'video/wmv', 'video/rmvb'].indexOf(file.type) === -1) {
        this.$message.error('请上传正确的视频格式')
        return false
      }
      if (!isLt300M) {
        this.$message.error('上传视频大小不能超过300MB哦!')
        return false
      }
    },
    // 视频上传过程中执行
    uploadVideoProcess (event, file, fileList) {
      this.Plus = false
      this.videoFlag = true
      this.videoUploadPercent = file.percentage.toFixed(0)
    },
    // 视频上传成功是执行
    handleVideoSuccess (res,file) {
      this.Plus = false
      this.videoUploadPercent = 100
      
    //   this.$axios.get('/api/detectVideo').then(res=>{
    //   alert("111");
    //   this.url = res.url;
    //   alert(this.url);
    //   //将api中College数据读入到此文件中的College数据中
    // })
    // .catch(erroe=>{
    //   alert("error");
    // })
    this.viurl=res.url;
      //  alert(this.viurl);
      // Echo("/api/detectVideo").data(file)
                     
      //   .then((res)=>{
      //       this.url=res.url;
      //    });
      // alert(this.url);
      // // 如果为200代表视频保存成功
      // if (res.resCode === '200') {
      //   // 接收视频传回来的名称和保存地址
      //   // 至于怎么使用看你啦~
      //   this.videoForm.videoId = res.newVidoeName
      //   this.videoForm.videoUrl = res.VideoUrl
      //   this.$message.success('视频上传成功！')
      // } else {
      //   this.$message.error('！')
      // }
    }
  }
}
</script>

