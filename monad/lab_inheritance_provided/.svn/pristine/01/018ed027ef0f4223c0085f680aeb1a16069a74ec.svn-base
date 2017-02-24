// Cs 225 Lab04 Unit Tests

#include "drawable.h"
#include "color.h"
#include "vector2.h"
#include "shape.h"
#include "circle.h"
#include "triangle.h"
#include "rectangle.h"
#include "truck.h"
#include "flower.h"

#include "proxy.h"

#if MP_PART(0)

UNIT_TEST(test_virtual_perim, 10, 10, 1000)
{
    Vector2 rectangle_center(4,5);
    Rectangle * rectangle = new Rectangle(rectangle_center, color::BLUE,
            32, 32);

    Shape * shape = rectangle;

    int rect_perim = rectangle->perimeter();
    int shape_perim = shape->perimeter();

    delete rectangle;

    ASSERT(rect_perim == shape_perim);
    PASS;
}

UNIT_TEST(test_virtual_area, 10, 10, 1000)
{
    Vector2 rectangle_center(4,5);
    Rectangle * rectangle = new Rectangle(rectangle_center, color::GREEN,
            47, 47);

    Shape * shape = rectangle;

    int rect_area = rectangle->area();
    int shape_area = shape->area();

    delete rectangle;

    ASSERT(rect_area == shape_area);
    PASS;
}

VALGRIND_TEST(test_destructor, 20, 20, 10000)
{
    Shape * triangle = new Triangle(color::ORANGE, Vector2(8,7), Vector2(5,5),
            Vector2(9,9));
    delete triangle;

    PASS;
}

UNIT_TEST(test_constructor, 20, 20, 1000)
{
    Circle circle(Vector2(26,26), color::ORANGE, 3);
    if( circle.color().red != color::ORANGE.red )
        FAIL("Circle's red color component is incorrect.");

    if( circle.color().blue != color::ORANGE.blue )
        FAIL("Circle's blue color component is incorrect.");

    if( circle.color().green != color::ORANGE.green )
        FAIL("Circle's green color component is incorrect.");

    PASS;
}

VALGRIND_TEST(test_drawable_destructor, 10, 10, 10000)
{
    Drawable * truck = new Truck(Vector2(123,456));
    delete truck;

    PASS;
}

UNIT_TEST(test_truck_draw, 10, 10, 1000)
{
    PNG canvas;
    canvas.resize(128, 128);

    PNG soln;

    Drawable * truck = new Truck(Vector2(64, 64));
    truck->draw(&canvas);

    soln.readFromFile("soln_truck.png");

    delete truck;

    ASSERT(canvas == soln);
    PASS;
}

UNIT_TEST(test_slicing, 20, 20, 1000)
{
    PNG canvas;
    canvas.resize(128,128);

    PNG soln;
    PNG soln_old;
    soln.readFromFile("soln_flower.png");
    soln_old.readFromFile("soln_flower_old.png");

    Drawable * flower = new Flower(Vector2(64,64));

    flower->draw(&canvas);

    delete flower;

    if (canvas != soln && canvas != soln_old)
		FAIL("Drawn Flower does not match solution");

    PASS;
}

VALGRIND_TEST(test_flower_destructor, 10, 10, 10000)
{
    Drawable * flower = new Flower(Vector2(64,64));
    delete flower;

    PASS; 
}

#endif
