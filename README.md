# Lưu ý
- Code được thực hiện trên Visual Studio 2019, SFML 2-5-1 32 bit (có thể tải bản 2017).
- Trong repo, đã upload sẵn SFML và đã configure (test trên VS2019, VS2022) nên "có vẻ" không cần cài gì thêm, nếu chạy thử file có vấn đề thì report lại.
- Đã add .gitignore, bao gồm test.cpp nên nếu cần thiết, có thể test project trên file này.
- Bối cảnh chung là galaxy.
- Trong lúc tìm có vẻ tài liệu tham khảo khá nhiều, nếu chỗ nào không rõ / thắc mắc / cảm thấy rườm rà thì cứ nhắn lên.
- Vì máy Dũng cùi quá nên hi vọng các bạn sử dụng ảnh có kích thước nhỏ, ví dụ background gốc (960 x 1920) phải scale về (350 x 700) :( (tạm thời cứ thể này đã).

# Công việc
- Hoàng:
	- Tạo class `Object` (Object.h), có 2 class kế thừa là `Obstacle` và `Spawner` (chắc phải nghiên cứu tiếp có phải kế thừa không).
	- Trong class `Spawner` cần có 
		- `Constructor` nhận vào: __srcImg__ (string dẫn đến file ảnh), __srcSound__ (string dẫn đến file âm thanh), __width__ (int), __height__ (int) (lần lượt là kích thước của vật thể).
		- Hàm `moveHorizontal` nhận vào: __coorX__ (int), __coorY__ (int) là tọa độ điểm xuất phát, __speed__ (int) là tốc độ, __toRight__ (bool) (quy định chiều chuyển động), lưu ý, sau khi đi hết biên trái/phải, nó sẽ xuất hiện lại ở biên phải/trái.
		- (_optional_) Hàm `moveCircular` nhận vào: __coorX__ (int), __coorY__ (int) là tọa độ điểm xuất phát, __centerX__ (int), __centerY__ (int) là tọa độ tâm đường tròn, __speed__ (int) (tốc độ) (tùy ý định nghĩa), __clockwise__ (chiều chuyển động).
		- Hàm `onSound`, `offSound` không nhận vào tham số để tắt mở nhạc.
		- Hàm `stop` không nhận vào tham số để dừng lại.
		- Phần âm nhạc và hình ảnh, có thể tùy ý tìm kiếm.
		- Tìm mấy hình kiểu alien, spaceship,...
	- Trong class `Obstacle` cần có
		- `Constructor` nhận vào: __srcImg__ (string dẫn đến file ảnh), __srcSound__ (string dẫn đến file âm thanh), __width__ (int), __height__ (int) (lần lượt là kích thước của vật thể).
		- Hàm `place` nhận vào: __coorX__ (int), __coorY__ (int) là tọa độ vật thể (vật đứng yên).
		- Hàm `collapse` : phát ra âm thanh đập vào, kêu bing boong gì đó (dựa vào file âm thanh nói trên).
		- Có thể sử dụng hình kiểu planet, thiên thạch, ... (nên đưa ra dạng hình vuông cho đa dạng thẩm mỹ).
	- Tạo class `Item` (Item.h)
		- `Constructor` nhận vào: __srcImg__ (string dẫn đến file ảnh), __width__ (int), __height__ (int) (lần lượt là kích thước của vật thể), __type__ (kiểu dữ liệu SpecialItem được định nghĩa có HEART, FROZEN, INVISIBLE và GOAL).
		- Hàm `place` nhận vào: __coorX__ (int), __coorY__ (int) là tọa độ vật thể (vật đứng yên).
		- Hình ảnh có thể tự kiếm.
		- Đại ý HEART là thêm mạng, FROZEN là đóng băng, dừng lại toàn bộ di chuyển, INVISIBLE là đi xuyên qua mấy cái kia (ứ biết dùng tên gì nữa :(), GOAL là đích.
- Duy:
	- Tạo class `Player`
		- `Constructor` nhận vào __srcImg__ (string dẫn đến file ảnh), __width__ (int), __height__ (int) (lần lượt là kích thước của vật thể).
		- __speed__ để cố định một số, số nào thì chắc thử thực tế cho dễ hình dung.
		- 4 hàm `move` cơ bản.
		- Một vector lưu những Item đang có (dựa vào vector này biết mình có mấy mạng luôn).
		- Hàm `useInvisible` để làm mờ đi chính mình (phục vụ INVISIBLE, chỉ chạy khi có item này trong người).
- Các class trên đều đi kèm với `friend function`
```c
friend void drawObject(sf::RenderWindow &window, ..object..) {
	window.draw(..object..);
}
// Chắc là thế, chưa test thực tế.
```
- Code tham khảo cho dễ nhìn sơ qua (mình sẽ sử dụng background kiểu này): https://drive.google.com/drive/folders/1r9QRgxVx61quz0DssRqDb-U9xdJAjFis?usp=sharing