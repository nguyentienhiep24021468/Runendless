# TOWER FORTRESS
link video demo: 
# Giới thiệu về game
Tower Fortress là dạng game nhập vai. Bạn bị mắc kẹt trong 1 mê cung và bạn cần phải thoát ra, trong mê cung sẽ có những con vật muốn giết bạn, 1 cái gai khổng lồ đuổi theo bạn, và những cái hố sẽ khiến bạn chết nếu rơi xuống. Việc của bạn là đi đến cuối mê cung để thoát ra ngoài.
# cách tải game 
cách 1: download file.zip theo đường link sau: https://github.com/nguyentienhiep24021468/Runendless 

cách 2: tải trực tiếp code về, tải các file ảnh và âm thanh về sau đó cài các sdl liên quan. 

# cách thành phần chính trong game:
![nhân vật](picture/1.png) 
![quái](picture/botdeath1.png)
![gai lớn](picture/spike1.png)

Khi bắt đầu game một menu sẽ hiện ra:
![menu](picture/menu.png)
có thể bấm chuột vào biểu tượng hình play để chơi
khi chết menu die sẽ hiện ra 
![menudie](picture/menudie.png)
bấm vào replay để chơi lại ngay, menu thì sẽ về giao diện lúc đầu game;

# cơ chế hoạt động
-khi mới run code màn hình menu sẽ hiện ra với nhạc nền;
-
-khi vào game play:
  -
  di chuyển bằng    a:sang trái    d:sang phải    space:nhảy     j:bắn      k:nhảy
  -
-Sẽ có những map và những con quái ở đó bạn cần bắn chết hoặc nhảy qua nó để đi tiếp;
-
-Con bot sẽ biết bạn đang ở gần nếu bạn nằm trong bán kính mà nó check được, nếu check được nó sẽ lập tức tăng tốc đến chỗ bạn và chém bạn;
-
-Về cái gai khổng lồ nó chỉ bắt đầu di chuyển sau khi bạn bấm vào game và thực hiện 1 hành động nào đó(để tránh bấm play bị lag xong bị gai cán chết ngay )
-
-Khi đuổi theo bạn nếu bạn chạy quá nhanh và gai sắp tuột ra khỏi màn hình thì gai sẽ tự tăng tốc để luôn ở trong màn hình(nếu để bạn đi xa quá thì sẽ không còn kịch tính)
-
-Gai khi cán vào con bot cũng sẽ khiến con bot chết;
-
-Ngoài ra gai sẽ dừng lại trước 4 tiled ở cuối map và tiếp tục xoay ở đó, người chơi không kịp về đích thì chỉ cần đi đến đó là an toàn;
-
-Player sẽ chết khi bị bot chém, chạm vào gai, rơi xuống đáy màn hình ( map sẽ có những chỗ không có chỗ đứng );
-
-Khi chết âm thanh gameover hiện lên và menudie sẽ hiện lên cho bạn 2 lựa chọn 1 là về menu chính 2 là chơi lại ngay;
-
-Khi thắng âm thanh victory hiện lên và menudie cũng sẽ lên cho bạn 2 lựa chọn tương tự;
-
-Hơn nữa trong xuốt quá trình chơi game sẽ luôn có nhạc;
-

# Đồ họa:
-ảnh nhân vật và con bot và cái gai tìm trên mạng
-
-map thì tìm các phiến đá vuông 64*64 và tự vẽ bằng tiled sau đó edit bằng libspire
-
-âm thanh: mạng
-
-Tất cả các file đều free trên mạng và có trong picture.zip và music.zip;
-

# Ý tưởng:
-Chính: fakemg (youtube)
-

# Nguồn tham khảo:
-Vì game là private nên không tìm được source công khai nên không tìm được code có sẵn;
-
