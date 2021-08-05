export module GLWindow:AssisantType;

// 储存颜色信息
struct BackgroundColor {
    float r;
    float g;
    float b;
    float a;
};
// 生成并将属性值绑定到VBO
struct AssistStruct {
    int location;  // 属性值在的位置
    int size;  // 属性值在数组中的步长
};
