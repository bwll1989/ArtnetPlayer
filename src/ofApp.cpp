#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    message = "Settings.xml loaded!";
    if( XML.loadFile("Settings.xml") )
    {
        message = "Settings.xml loaded!";
    }else{
        message = "unable to load mySettings.xml check data/ folder";
    }
    ofSetWindowTitle("ArtnetPlayer");
    
    WindowWidth=XML.getValue("Settings:ArtNet:WindowW", 512);
    //窗口宽
    WindowHeight=XML.getValue("Settings:ArtNet:WindowH", 512);
    //窗口高
    showGUI=XML.getValue("Settings:ArtNet:Gui", true);
    //显示窗口控件
    port=XML.getValue("Settings:ArtNet:Port", 6454);
    //设置端口
    net=XML.getValue("Settings:ArtNet:Net", 0);
    //设置artnet net
    subnet=XML.getValue("Settings:ArtNet:Subnet", 0);
    //设置artnet subnet
    firstUniverses=XML.getValue("Settings:ArtNet:FirstUniverse", 0);
    amntUniverses=XML.getValue("Settings:ArtNet:AmntUniverse", 16);
    //设置最大Universe数量，数量太多会造成帧率下降
//    cout<<amntUniverses<<endl;
    videoName = XML.getValue("Settings:ArtNet:VideoName", "Artnet.mov");
    //设置视频名称
    cout<<videoName<<endl;
    
    playVideo=XML.getValue("Settings:ArtNet:AutoPlay", true);
    //设置自动播放

    Node_IP = XML.getValue("Settings:ArtNet:Destination", "255.255.255.255");
    //设置Artnet单播目的地址，默认为广播
    cout<<Node_IP<<endl;
    ofSetFrameRate(XML.getValue("Settings:ArtNet:Fps", 30));
    //设置传输帧率默认为30，上限为视频帧率
    artnetNode_IP.setup(Node_IP, port);
//    artnetNode_IP.enableThread(XML.getValue("Settings:ArtNet:Fps", 10));
    artnetNode_IP.enableThread(100.0);
//    cout<<XML.getValue("Settings:ArtNet:Fps", 10)<<endl;
    gui = new ofxDatGui( ofxDatGuiAnchor::TOP_RIGHT );
    gui->setWidth(WindowWidth);
    gui->addHeader(":: drag me to reposition ::");
    //设置可拖拽
    gui->addFRM();
    //显示fps
    gui->addTextInput("TotalFrames&CurrentFrames", "");
    gui->addTextInput("FirstUniverses&TotalUniverses", "");
//    gui->addBreak();
    gui->addSlider("Progress", 0, 100, 100);
    gui->addSlider("Jump to", 0, 100, 0);
    gui->addButton("click");
    gui->setVisible(showGUI);
    ofSetFullscreen(false);
    ofSetWindowShape(WindowWidth,WindowHeight);
    ofSetWindowPosition(ofGetScreenWidth()/2-WindowWidth/2, ofGetScreenHeight()/2-WindowHeight/2);

    gui->onButtonEvent(this, &ofApp::onButtonEvent);
    gui->onSliderEvent(this, &ofApp::onSliderEvent);
    loadVideo();
    if(playVideo==true)
    //设置自动播放后，软件开启后自动播放视频
    {
        mPlayer.play();
        gui->getButton("click")->setLabel("Playing");
    }else{
        gui->getButton("click")->setLabel("Stop");
    }
    
}

//--------------------------------------------------------------
void ofApp::loadVideo(){
    
    std::string fullPathVideo = "videos/" + videoName;
    gui->getHeader()->setLabel(fullPathVideo);
//    cout << fullPathVideo << endl;
    mPlayer.load(videoName);
    mPlayer.setLoopState(OF_LOOP_NONE);
    ofPixels & pixels = mPlayer.getPixels();
    //读取播放器像素值
    videoSize.x = pixels.getWidth();
    //视频宽度
    videoSize.y = pixels.getHeight();
    //视频高度
    videoChannels = pixels.getNumChannels();
    //视频通道数
    cout<<videoChannels<<endl;
    //颜色通道数
    cout << videoSize.y << " " << videoSize.x <<videoChannels<< endl;
//
    gui->getSlider("Jump to")->setMax(mPlayer.getTotalNumFrames());
//    f.clear();
//    f.allocate(512, 1, GL_RGB);
//    //创建1*512的FBO，使用FBO效率低下

//    cout << "f.width = " << f.getWidth() << " f.height = " << f.getHeight() << endl;
}



//--------------------------------------------------------------
void ofApp::update(){
    if(ofGetWidth()!=WindowWidth||ofGetWidth()!=WindowHeight){
        ofSetWindowShape(WindowWidth, WindowHeight);
    }
    //禁用窗口缩放
    sync.update();
    mPlayer.update();
    gui->getSlider("progress")->setValue(mPlayer.getPosition()*100);
    gui->getTextInput("TotalFrames&CurrentFrames")->setText(ofToString(mPlayer.getTotalNumFrames())+"/"+ofToString(mPlayer.getCurrentFrame()));
    gui->getTextInput("FirstUniverses&TotalUniverses")->setText(ofToString(firstUniverses)+"/"+ofToString(amntUniverses));
    sendArtnet(mPlayer.getPixels());
    //调用sendArtnet，将播放器的数据发送出去
    

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetBackgroundColor(0, 0, 0);
    mPlayer.draw(0, 0,WindowWidth,videoSize.y);
}

//--------------------------------------------------------------
void ofApp::sendArtnet(ofPixels & pixels){
    for (int j = firstUniverses; j < firstUniverses+amntUniverses; j += 1){
        dataToSend=mPlayer.getPixels().getLine(j).asPixels();
        //逐行读取像素值，并转存值dataToSend
        ofxArtnetMessage m(dataToSend.getChannel(0));
        //将读取后像素值中R通道提取出来，保存至ArtnetMessage m
        m.setUniverse(subnet, net, j);//Progress,net, universe
        //设置ArtnetMessage m的subnet,net, universe
        artnetNode_IP.sendArtnet(m);
    }

}
void ofApp::onButtonEvent(ofxDatGuiButtonEvent(e)){
    if(e.target->is("click")){
        if(mPlayer.isPlaying()){
            mPlayer.setPaused(true);
            e.target->setLabel("Pause");
           
        }else if(mPlayer.isPaused())
        {
            mPlayer.play();
            e.target->setLabel("Playing");
        }
        
    }
}
void ofApp::onSliderEvent(ofxDatGuiSliderEvent e)
{
    if (e.target->is("Jump to")){
        gui->setOpacity(e.scale);
        if(mPlayer.isPlaying())
        {
            mPlayer.stop();
            mPlayer.setFrame(e.value);
            mPlayer.play();
        }else{
            mPlayer.stop();
            mPlayer.setFrame(e.value);
        }


    }
        
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key =='p'||key==' ')
    //P或者空格播放暂停视频
    {
        if(mPlayer.isPlaying()){
            mPlayer.setPaused(true);
            gui->getButton("click")->setLabel("Pause");
           
        }else if(mPlayer.isPaused())
        {
            mPlayer.play();
            gui->getButton("click")->setLabel("Playing");
        }
    }else if (key=='g')
    //G键显示隐藏UI
    {
        showGUI=!showGUI;
        gui->setVisible(showGUI);
    }else if (key=='s')
    //S键停止视频
    {
        mPlayer.stop();
        gui->getButton("click")->setLabel("Stop");
    }
    
}
