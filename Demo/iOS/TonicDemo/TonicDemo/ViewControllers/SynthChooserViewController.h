//
//  SynthChooserViewController.h
//  TonicDemo
//
//  Created by Nick Donaldson on 2/8/13.

//

#import <UIKit/UIKit.h>
#import <CoreMotion/CoreMotion.h>

@interface SynthChooserViewController : UIViewController <UITableViewDataSource, UITableViewDelegate>
@property (weak, nonatomic) IBOutlet UITableView *synthTableView;
@end
