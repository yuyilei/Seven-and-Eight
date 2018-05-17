swagger: "2.0"

info:
  version: 1.0.0
  title: MusicRunning Document
  description: MusicRunning API Document

schemes:
  - https
host: simple.api
basePath: /api


paths:
  /login/:
    post:
      summary: 用户登录
      description: login
      parameters: 
      - name: logininfo
        in: body
        description: 登录所需用户名密码
        required: true
        schema:
          required: 
            - username
            - password
          properties:
            password:
              type: string
            username:
              type: string
      responses:
        200:
          description: 登录成功
          schema:
            required: 
              - token
            properties:
              token:
                type: string
        401:
          description: 密码错误 or 用户不存在
          
          
  /signup/:
    post:
      summary: 用户注册
      description: signup
      parameters: 
      - name: signup
        in: body
        description: 注册所需用户名密码
        required: true
        schema:
          required: 
            - username
            - password
          properties:
            password:
              type: string
            username:
              type: string
      responses:
        200:
          description: 注册成功
        403:
          description: 用户名占用 
          
 
 
  /user/{uid}/:
    get:
      summary: 获取某一用户的信息
      parameters: 
        - in: header
          name: token
          description: 验证用户身份
          required: true
          type: string
        - in: path
          name: uid
          description: 用户ID
          required: true
          type: integer
      responses:
        200:
          description: 成功
          schema:
            $ref: '#/definitions/UserInfo'
        403:
          description: token验证错误
        404:
          description: 用户不存在 
            
    
  /user/{uid}/info/:
    post:
      summary: 修改某一用户的信息
      parameters: 
        - in: header
          name: token
          description: 验证用户身份
          required: true
          type: string
        - in: path
          name: uid
          description: 用户ID
          required: true
          type: integer
        - in: body
          name: info
          schema:
            $ref: '#/definitions/EditUserInfo'
          
      responses:
        200:
          description: 用户信息修改成功
        403:
          description: token验证错误，不能修改 
          
              

  
  /user/distance/:
    post:
      summary: 记录用户跑步的里程数
      parameters: 
        - in: header
          name: token
          description: 标示用户信息的token
          required: true
          type: string
        - in: body
          name: distanceinfo
          description: 用户跑步的距离
          required: true
          schema:
            required:
              - date
              - distance
            properties:
              distance:
                description: 跑步距离
                type: integer
              date:
                type: string 
                description: 日期 
      responses:
        200:
          description: 记录用户跑步距离成功
        403:
          description: token验证错误，不能记录
          
    

          
    
  /circle/{cid}/people/:
    get:
      summary: 获取某一圈子里的人的排行榜 (佑铭，南湖，高职)
      parameters: 
        - in: path 
          name: cid 
          description: 圈子的ID
          required: true
          type: integer
      responses:
        200: 
          description: 圈子的信息
          schema:
            type: array
            items:
                $ref: '#/definitions/UserRunInfo'
          
        
          
          
  /location/:
    get:
      summary: 获取用户的位置信息
      parameters: 
        - in: header
          name: token
          description:  标识身份的Token
          required: true
          type: string
      responses:
        200:
          description: location info 
          schema:
            required: 
              - x
              - y
            properties:
              x:
                type: string 
              y:
                type: string 
                
  /greet/:
    post: 
      summary: 向某人打招呼
      parameters: 
        - in: header
          name: token 
          description: 标示身份的token
          required: true
          type: string
        - in : body
          name: greetwho 
          description: 打招呼的对象和自己
          required: true
          schema:
            required:
              - towho
              - me
            properties:
              towho:
                description: 打招呼的对象的uid
                type: integer
              me:
                type: integer 
                description: 自己的uid 
      responses:
        200:
          description: 用户信息修改成功
        403:
          description: token验证错误
          
          
  /match/: 
    get: 
      summary: 获取与某人最匹配的对象
      parameters: 
        - in: header
          name: token 
          description: 标示身份的token
          required: true
          type: string
      responses:
        200:
          description: 成功
          schema:
            $ref: '#/definitions/User'
        403:
          description: token验证错误
        
    
    
  /match/{uid}/rate/: 
    get: 
      summary: 与某人的匹配值
      parameters:
        - in: path 
          name: uid 
          description: 对象的ID
          required: true
          type: integer
        - in: header
          name: token 
          description: 标示身份的token
          required: true
          type: string
      responses:
        200:
          description: 成功
          schema:
            required: 
              - rate 
            properties:
              rate: 
                description: 匹配度
                type: integer
        403:
          description: token验证错误
          
        
    
          
definitions:
  User:
    required: 
      - uid
      - username 
      - signature
      - avatar 
    properties:
      uid:
        description: 用户ID
        type: integer
      username:
        description: 用户名
        type: string
      signature:
        description: 用户信息
        type: string
      avatar:
        description: 用户头像URL
        type: string
        
        
  UserRunInfo:
    required: 
      - uid
      - username
      - speed
      - distance  
      - department 
      - gender 
      - grade 
    properties:
      uid:
        description: 用户ID
        type: integer
      username:
        description: 用户名
        type: string
      distance:
        description: 用户里程数
        type: integer
      department:
        description: 学院
        type: string
      gender:
        description: 性别
        type: string
      grade:
        description: 年级
        type: integer
        
        
  UserInfo:
    required: 
      - username
      - avatar
      - uid 
      - distance 
      - signature 
      - department 
      - gender 
      - grade 
    properties:
      uid:
        description: 用户ID
        type: integer
      username:
        description: 用户名
        type: string
      avatar:
        description: 头像
        type: string
      distance:
        description: 用户跑步距离
        type: integer
      signature:
        description: 用户签名 
        type: string
      department:
        description: 学院
        type: string
      gender:
        description: 性别
        type: string
      grade:
        description: 年级
        type: integer
        
        
  EditUserInfo:
    required: 
      - username
      - avatar
      - signature 
      - department 
      - gender 
      - grade 
    properties:
      username:
        description: 用户名
        type: string
      avatar:
        description: 头像
        type: string
      signature:
        description: 用户签名 
        type: string
      department:
        description: 学院
        type: string
      gender:
        description: 性别
        type: string
      grade:
        description: 年级
        type: integer
        
      
      
        
        

