# Test-Game

上一次忘记写这个文件了  

2022/3/8/12:00 完成了：  

1. opengl绘制游戏的框架
2. followcamera。
3. 图元绘制以及实体绑定图元。
4. 玩家的上下左右基本操作。
5. 基本的物理碰撞。
6. 世界的读取和预制物的读取。

2022/3/8/23:19 完成了：  

1. 事件监听器，PeriodicTakser和TimeTasker（不知怎么描述，用来处理循环事件和延迟事件的）
2. 玩家可以射箭（其实是测试上面第一条有没有做对的）
3. 修复了一些物理的BUG（然而还有）

2022/3/10/1:27 完成了：   

1. 优化地图读取   
2. 修复射箭BUG和更多的物理BUG   
3. Component组件基类，以及Combat和Health两个战斗常用组件   
4. 基本的怪物基类和一个实验性的怪物（不会动）   
5. 攻击逻辑（验证上述2.3.4正确）   

2022/3/11/1:37 完成了：   

1. 怪物受击效果和人物受击效果   
2. 更加完善的更新Position，Rotation 和 Scaling 的流程   
3. 添加了怪物的Brains，怪物可以按照设定的行动优先顺序移动   
4. 两个基本怪物完成（会飞和不会飞）   
至此应当说完成了基本任务把（笑   