/*========== my_main.c ==========
  This is the only file you need to modify in order
  to get a working mdl project (for now).
  my_main.c will serve as the interpreter for mdl.
  When an mdl script goes through a lexer and parser, 
  the resulting operations will be in the array op[].
  Your job is to go through each entry in op and perform
  the required action from the list below:
  frames: set num_frames (in misc_headers.h) for animation
  basename: set name (in misc_headers.h) for animation
  vary: manipluate knob values between two given frames
        over a specified interval
  set: set a knob to a given value
  
  setknobs: set all knobs to a given value
  push: push a new origin matrix onto the origin stack
  
  pop: remove the top matrix on the origin stack
  move/scale/rotate: create a transformation matrix 
                     based on the provided values, then 
		     multiply the current top of the
		     origins stack by it.
  box/sphere/torus: create a solid object based on the
                    provided values. Store that in a 
		    temporary matrix, multiply it by the
		    current top of the origins stack, then
		    call draw_polygons.
  line: create a line based on the provided values. Store 
        that in a temporary matrix, multiply it by the
	current top of the origins stack, then call draw_lines.
  save: call save_extension with the provided filename
  display: view the image live
  
  jdyrlandweaver
  =========================*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "parser.h"
#include "symtab.h"
#include "y.tab.h"

#include "misc_headers.h"
#include "matrix.h"
#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "stack.h"

/*======== void first_pass()) ==========
  Inputs:   
  Returns: 
  Checks the op array for any animation commands
  (frames, basename, vary)
  
  Should set num_frames and basename if the frames 
  or basename commands are present
  If vary is found, but frames is not, the entire
  program should exit.
  If frames is found, but basename is not, set name
  to some default value, and print out a message
  with the name being used.
  jdyrlandweaver
  ====================*/
void first_pass() {
  int i;
  int f,n,v = 0;
  for (i = 0; i < lastop; i++){
    switch (op[i].opcode){
      case FRAMES:
	num_frames = op[i].op.frames.num_frames;
	f = 1;
	break;
      case BASENAME:
	strcpy(name,op[i].op.basename.p->name);
	n = 1;
	break;
      case VARY:
	v = 1;
	break;
    }
  }
  if (v && !f)
    exit(0);
  if (f && !n){
    strcpy(name,"anim");
    printf("Basename not inputted, using anim as basename.\n");
  }
  //printf("first pass\n");
}

/*======== struct vary_node ** second_pass()) ==========
  Inputs:   
  Returns: An array of vary_node linked lists
  In order to set the knobs for animation, we need to keep
  a separate value for each knob for each frame. We can do
  this by using an array of linked lists. Each array index
  will correspond to a frame (eg. knobs[0] would be the first
  frame, knobs[2] would be the 3rd frame and so on).
  Each index should contain a linked list of vary_nodes, each
  node contains a knob name, a value, and a pointer to the
  next node.
  Go through the opcode array, and when you find vary, go 
  from knobs[0] to knobs[frames-1] and add (or modify) the
  vary_node corresponding to the given knob with the
  appropirate value. 
  05/17/12 09:29:31
  jdyrlandweaver
  ====================*/
struct vary_node ** second_pass() {

  struct vary_node ** knobs;
  knobs = (struct vary_node **)malloc( num_frames * sizeof(struct vary_node *));
  
  int i,j;
  for(i = 0; i < num_frames; i++)
    knobs[i]=(struct vary_node *) NULL;
  for(i = 0; i < lastop; i++) {
    switch (op[i].opcode){
      case VARY:
	for(j = 0;j<num_frames;j++){
	  
	  //set up node to be added
	  struct vary_node *new;
	  new = (struct vary_node *)malloc(sizeof(struct vary_node));

	  strcpy(new->name , op[i].op.vary.p->name);
	  new->next = (struct vary_node*) NULL;
	  //new->occupied = 1;
	  
	  int start_f = op[i].op.vary.start_frame;
	  int end_f = op[i].op.vary.end_frame;
	  double start_v = op[i].op.vary.start_val;
	  double end_v = op[i].op.vary.end_val;

	  //set value depending on frame j
	  
	  if((j>=start_f && j<=end_f)){
	    new->value = (end_v-start_v)*(j-start_f)/(end_f-start_f) + start_v;
	    //printf("%s Frame: %d, %f\n",new->name,j,new->value);
	  
	  //see where this node fits
	  if(!knobs[j]||strcmp(knobs[j]->name,new->name)==0){
	    //printf("%s if\n", new->name);
	      //printf("%s\n",new->name);
	      if(knobs[j])
		new->next = knobs[j]->next;
	      knobs[j]=new;
	    
	  }
	  else{
	    //printf("%s else\n",new->name);
	    //printf("%d, %f\n",j,new->value);
	    struct vary_node *curr;
	    curr=knobs[j];
	    while(curr){
	      //printf("%s\n",new->name);
	      if(!curr->next || strcmp(curr->next->name,new->name)==0){
		//printf("%s\n",new->name);
		if(curr->next)
		  new->next = curr->next->next;
		curr->next=new;
		curr = NULL;
	      }
	      else
		curr=curr->next;
	    }
	    
	  }}
	  else{
	    struct vary_node * placeholder;
	    placeholder = (struct vary_node *)malloc(sizeof(struct vary_node));
	    strcpy(placeholder->name,new->name);
	    placeholder->value = 99999999;
	    
	    if(!knobs[j]||strcmp(knobs[j]->name,placeholder->name)==0){
	      if(!knobs[j])
		knobs[j]=placeholder;
	    }
	  else{
	    
	    struct vary_node *curr;
	    curr=knobs[j];
	    while(curr){
	      if(!curr->next || strcmp(curr->next->name,placeholder->name)==0){
		if(!curr->next)
		  curr->next=placeholder;
		else
		  curr = NULL;
	      }
	      else
		curr=curr->next;
	    }
	    
	  }
	  }
	  }
	  //printf("NAME:%s\tFRAME: %d\tVAL:%f\n",new->name,j,new->value);
	  //print_knobs();
	}
    }
  //fill in the 99999999s
  //printf("here?\n");
  struct vary_node *curr,*head;
  head = knobs[0];
  //printf("here!\n");
  double tmp;
  int c = 0;
  while(head){
    tmp = 99999999;
    //printf("bleh\n");
    for(i = 0; i < num_frames; i++){
      struct vary_node *curr;
      curr = knobs[i];
      int d;
      for(d = 0; d < c; d++)
	curr = curr->next;
      if(curr->value<99999998){
	tmp = curr->value;
	//	printf("setting temp %d, %s tmp: %f\n",i,curr->name,curr->value);
      }
      else{
	//	printf("%d, %s tmp: %f\n",i,curr->name,tmp);
	curr->value = tmp;
      }
    }
    
    tmp = 99999999;
    for(i = num_frames-1; i >=0; i--){
      struct vary_node *curr;
      curr = knobs[i];
      int d;
      for(d = 0; d < c; d++)
	curr = curr->next;
      if(curr->value<99999998)
	tmp = curr->value;
      else{
	//printf("tmp: %d\n",tmp);
	curr->value = tmp;
      }
      }
    head = head->next;
    c+=1;
  }
  //printf("here!\n");
  //symtab and print stuff for testing
  /*for(i = 0; i < num_frames; i++){
    struct vary_node *curr;
    curr = knobs[i];
    while(curr){
      set_value(lookup_symbol(curr->name),curr->value);
      //curr=curr->next;
      // printf("NAME: %s\tFRAME: %d\tVAL: %f\n",curr->name,i,curr->value);
      curr=curr->next;
      }
    //print_knobs();
  }*/
  //print_knobs();
  if(!knobs[0])
    knobs=NULL;
  return knobs;
}

/*======== void print_knobs() ==========
Inputs:   
Returns: 
Goes through symtab and display all the knobs and their
currnt values
jdyrlandweaver
====================*/
void print_knobs() {
  
  int i;

  printf( "ID\tNAME\t\tTYPE\t\tVALUE\n" );
  for ( i=0; i < lastsym; i++ ) {

    if ( symtab[i].type == SYM_VALUE ) {
      printf( "%d\t%s  \t", i, symtab[i].name );

      printf( "SYM_VALUE\t");
      printf( "%6.2f\n", symtab[i].s.value);
    }
  }
}


/*======== void my_main() ==========
  Inputs:
  Returns: 
  This is the main engine of the interpreter, it should
  handle most of the commadns in mdl.
  If frames is not present in the source (and therefore 
  num_frames is 1, then process_knobs should be called.
  If frames is present, the enitre op array must be
  applied frames time. At the end of each frame iteration
  save the current screen to a file named the
  provided basename plus a numeric string such that the
  files will be listed in order, then clear the screen and
  reset any other data structures that need it.
  Important note: you cannot just name your files in 
  regular sequence, like pic0, pic1, pic2, pic3... if that
  is done, then pic1, pic10, pic11... will come before pic2
  and so on. In order to keep things clear, add leading 0s
  to the numeric portion of the name. If you use sprintf, 
  you can use "%0xd" for this purpose. It will add at most
  x 0s in front of a number, if needed, so if used correctly,
  and x = 4, you would get numbers like 0001, 0002, 0011,
  0487
  05/17/12 09:41:35
  jdyrlandweaver
  ====================*/
void my_main( int polygons ) {

  int i, f, j;
  double step;
  double xval, yval, zval;
  double knob_value = 1;
  //struct matrix *transform;
  //struct matrix *tmp = new_matrix(4, 1000);
  //struct stack *s = new_stack();
  screen t;
  color g;

  struct vary_node **knobs;
  struct vary_node *vn;
  char frame_name[128];
  num_frames = 1;
  first_pass();
  step = 5;
  // printf("here2\n");
  if(num_frames>1)
    knobs = second_pass();
  g.red = 0;
  g.green = 255;
  g.blue = 255;
  for(f = 0; f < num_frames; f++){

    //printf("here\n");
    
      struct matrix *transform;
      struct matrix *tmp = new_matrix(4, 1000);
      struct stack *s = new_stack();
      for(i = 0; i < lastop; i++){
      knob_value = 1;
      if(num_frames>1&&knobs)
	vn = knobs[f];
      //printf("here2\n");
      switch (op[i].opcode) {
      case AMBIENT:
        g.red = op[i].op.ambient.c[0];
	g.green = op[i].op.ambient.c[1];
	g.blue = op[i].op.ambient.c[2];
        break;
      case SPHERE:
	add_sphere( tmp,op[i].op.sphere.d[0], //cx
		    op[i].op.sphere.d[1],  //cy
		    op[i].op.sphere.d[2],  //cz
		    op[i].op.sphere.r,
		    step);
	//apply the current top origin
	matrix_mult( s->data[ s->top ], tmp );
	draw_polygons( tmp, t, g );
	tmp->lastcol = 0;
	break;

      case TORUS:
	add_torus( tmp, op[i].op.torus.d[0], //cx
		   op[i].op.torus.d[1],     //cy
		   op[i].op.torus.d[2],    //cz
		   op[i].op.torus.r0,
		   op[i].op.torus.r1,
		   step);
	matrix_mult( s->data[ s->top ], tmp );
	draw_polygons( tmp, t, g );
	tmp->lastcol = 0;
	break;

      case BOX:
	add_box( tmp, op[i].op.box.d0[0],
		 op[i].op.box.d0[1],
		 op[i].op.box.d0[2],
		 op[i].op.box.d1[0],
		 op[i].op.box.d1[1],
		 op[i].op.box.d1[2]);
	matrix_mult( s->data[ s->top ], tmp );
	draw_polygons( tmp, t, g );
	tmp->lastcol = 0;
	break;

      case LINE:
	add_edge( tmp, op[i].op.line.p0[0],
		  op[i].op.line.p0[1],
		  op[i].op.line.p0[1],
		  op[i].op.line.p1[0],
		  op[i].op.line.p1[1],
		  op[i].op.line.p1[1]);
	draw_lines( tmp, t, g );
	tmp->lastcol = 0;
	break;

      case MOVE:
	//printf("MOVING\n");
	//get the factors
	xval = op[i].op.move.d[0];
	yval =  op[i].op.move.d[1];
	zval = op[i].op.move.d[2];
	
	while(knobs&&num_frames>1&&vn&&op[i].op.move.p&&strcmp(vn->name,op[i].op.move.p->name))
	  vn=vn->next;
	//printf("HERE2\n");
	
	if(vn&&op[i].op.move.p){
	  knob_value = vn->value;
	  // printf("MOVING WORKS\n");
	}
	//printf("HERE3\n");
	xval *=knob_value;
	yval *=knob_value;
	zval *=knob_value;

	//printf("%s: %f\n",vn->name,yval);
	
	transform = make_translate( xval, yval, zval );
	//multiply by the existing origin
	matrix_mult( s->data[ s->top ], transform );
	//put the new matrix on the top
	copy_matrix( transform, s->data[ s->top ] );
	free_matrix( transform );
	break;

      case SCALE:
	//printf("SCALING\n");
	xval = op[i].op.scale.d[0];
	yval = op[i].op.scale.d[1];
	zval = op[i].op.scale.d[2];

	while(knobs&&num_frames>1&&vn&&op[i].op.scale.p&&strcmp(vn->name,op[i].op.scale.p->name))
	  vn = vn->next;

	if(vn&&op[i].op.scale.p)
	  knob_value = vn->value;

	xval *=knob_value;
	yval *=knob_value;
	zval *=knob_value;
	
	transform = make_scale( xval, yval, zval );
	matrix_mult( s->data[ s->top ], transform );
	//put the new matrix on the top
	copy_matrix( transform, s->data[ s->top ] );
	free_matrix( transform );
	break;

      case ROTATE:
	//printf("ROTATING\n");
	while(knobs&&num_frames>1&&vn&&op[i].op.rotate.p&&strcmp(vn->name,op[i].op.rotate.p->name)){
	  vn = vn->next;
	}

	if(vn&&op[i].op.rotate.p){
	  knob_value = vn->value;
	  //printf("WORKSSS\n");
	}
	    
	xval = op[i].op.rotate.degrees * ( M_PI / 180 ) * knob_value;

	//printf("%s: %f\n",vn->name,xval);

	//get the axis
	if ( op[i].op.rotate.axis == 0 ) 
	  transform = make_rotX( xval );
	else if ( op[i].op.rotate.axis == 1 ) 
	  transform = make_rotY( xval );
	else if ( op[i].op.rotate.axis == 2 ) 
	  transform = make_rotZ( xval );

	matrix_mult( s->data[ s->top ], transform );
	//put the new matrix on the top
	copy_matrix( transform, s->data[ s->top ] );
	free_matrix( transform );
	break;

      case PUSH:
	push( s );
	break;
      case POP:
	pop( s );
	break;
      case SAVE:
	save_extension( t, op[i].op.save.p->name );
	break;
      case DISPLAY:
	display( t );
	break;
      }
      //printf("here7\n");
    }
      //printf("here8\n");
    //print_symtab();
    free_stack( s );
    free_matrix( tmp );
    //free_matrix( transform );
    sprintf(frame_name, "./anim/%s%03d.png",name,f);
    save_extension( t, frame_name );
    clear_screen(t);
  }
}
