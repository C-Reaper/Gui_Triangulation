#include "/home/codeleaded/System/Static/Library/WindowEngine.h"
#include "/home/codeleaded/System/Static/Library/Delaunay.h"
#include "/home/codeleaded/System/Static/Library/Random.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#include <limits.h>

#define WIDTH			100.0f
#define HEIGHT			100.0f
#define POINT_RADIUS	1.0f

TransformedView tv;
Rect box;
Vector mesh;
Vector points;
Vector velocitys;
Vec2* selected;

int32_t Points_FindI(Vec2 p){
	for(int32_t i = 0;i<points.size;i++){
		const Vec2 point = *(Vec2*)Vector_Get(&points,i);
		const Vec2 delta = Vec2_Sub(p,point);
		const float dist2 = Vec2_Mag2(delta);
		
		if(dist2 < POINT_RADIUS * POINT_RADIUS){
			return i;
		}
	}
	return -1;
}
Vec2* Points_Find(Vec2 p){
	const int32_t id = Points_FindI(p);
	if(id >= 0) return (Vec2*)Vector_Get(&points,id);
	else		return NULL;
}

void Setup(AlxWindow* w){
	Random_Set(Time_Nano());

	mesh = Vector_New(sizeof(Triangle));
	points = Vector_New(sizeof(Vec2));
	velocitys = Vector_New(sizeof(Vec2));
	
	selected = NULL;
	box = Rect_New(
		(Vec2){ 0.0f,0.0f },
		(Vec2){ WIDTH,HEIGHT }
	);

	tv = TransformedView_Make(
		(Vec2){ GetWidth(),GetHeight() },
		(Vec2){ 0.0f,0.0f },
		(Vec2){ 0.01f,0.01f },
		(float)GetWidth() / (float)GetHeight()
	);
}
void Update(AlxWindow* w){
	tv.ZoomSpeed = (float)w->ElapsedTime;
	TransformedView_HandlePanZoom(&tv,window.Strokes,(Vec2){ GetMouse().x,GetMouse().y });
	TransformedView_Output(&tv,(Vec2){ GetWidth(),GetHeight() });
	const Vec2 wm = TransformedView_ScreenWorldPos(&tv,GetMouse());

	if(Stroke(ALX_MOUSE_L).PRESSED){
		selected = NULL;

		Vec2* point = Points_Find(wm);
		if(point)
			selected = point;
		else{
			const float angle = Random_f64_MinMax(0.0,F64_PI2);
			const float velo = Random_f64_MinMax(2.0,5.0);

			Vector_Push(&points,(Vec2[]){ wm });
			Vector_Push(&velocitys,(Vec2[]){ Vec2_Mulf(Vec2_OfAngle(angle),velo) });
		}
	}else if(Stroke(ALX_MOUSE_L).DOWN){
		if(selected) *selected = wm;
	}

	if(Stroke(ALX_MOUSE_R).DOWN){
		selected = NULL;
		const int32_t point_id = Points_FindI(wm);

		if(point_id >= 0){
			Vector_Remove(&points,point_id);
			Vector_Remove(&velocitys,point_id);
		}
	}

	for(int i = 0;i<points.size;i++){
		Vec2* const point = (Vec2*)Vector_Get(&points,i);
		Vec2* const velo = (Vec2*)Vector_Get(&velocitys,i);

		point->x += velo->x * w->ElapsedTime;
		point->y += velo->y * w->ElapsedTime;

		if(point->x < 0.0f){
			point->x = 0.0f;
			velo->x *= -1.0f;
		}else if(point->x > box.d.x){
			point->x = box.d.x;
			velo->x *= -1.0f;
		}

		if(point->y < 0.0f){
			point->y = 0.0f;
			velo->y *= -1.0f;
		}else if(point->y > box.d.y){
			point->y = box.d.y;
			velo->y *= -1.0f;
		}
	}

	Vector_Clear(&mesh);
	Delaunay_CreateMesh(box,&points,&mesh);

	Clear(BLACK);
	
	const Rect box_s = TransformedView_WorldScreenRect(&tv,box);
	Rect_RenderWire(WINDOW_STD_ARGS,box_s,WHITE,1.0f);
	Delaunay_Render(WINDOW_STD_ARGS,box,&tv,&mesh);
	
	const Vec2 size = TransformedView_WorldScreenLength(&tv,Vec2_Mulf(Vec2_Norm((Vec2){ 1.0f,1.0f }),POINT_RADIUS));
	for(int i = 0;i<points.size;i++){
		const Vec2 point = *(Vec2*)Vector_Get(&points,i);
		const Vec2 p1 = TransformedView_WorldScreenPos(&tv,point);
		//Circle_R_Render(WINDOW_STD_ARGS,p1,size,WHITE);
	}

	CStr_RenderAlxFontf(WINDOW_STD_ARGS,GetAlxFont(),0,0,RED,"X: %f, Y: %f",wm.x,wm.y);
	CStr_RenderAlxFontf(WINDOW_STD_ARGS,GetAlxFont(),0,window.font.CharSizeY + 1,RED,"Points: %d, Triangles: %d",points.size,mesh.size);
}
void Delete(AlxWindow* w){
	Vector_Free(&points);
	Vector_Free(&velocitys);
	Vector_Free(&mesh);
}

int main(){
	if(Create("Triangulation",2500,1440,1,1,Setup,Update,Delete))
        Start();
    return 0;
}