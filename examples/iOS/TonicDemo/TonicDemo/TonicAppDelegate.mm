//
//  TonicAppDelegate.m
//  TonicDemo
//
//  Created by Morgan Packard on 1/25/13.

// 

#import "TonicAppDelegate.h"
#import "TonicSynthManager.h"
#import "SynthChooserViewController.h"

@implementation TonicAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
  if ( [UINavigationBar instancesRespondToSelector:@selector(setBarTintColor:)] ) {
    [[UINavigationBar appearance] setBarTintColor:[UIColor blackColor]];
    [[UINavigationBar appearance] setBarStyle:UIBarStyleBlackTranslucent];
  }
  else {
    [[UINavigationBar appearance] setTintColor:[UIColor blackColor]];
  }
  [[UINavigationBar appearance] setTitleTextAttributes:@{UITextAttributeFont : [UIFont fontWithName:@"HelveticaNeue-Bold" size:20], UITextAttributeTextColor : [UIColor whiteColor]}];
  
  SynthChooserViewController *scVC = [[SynthChooserViewController alloc] initWithNibName:nil bundle:nil];
  self.rootNavController = [[UINavigationController alloc] initWithRootViewController:scVC];
  
    // Override point for customization after application launch.
  [[TonicSynthManager sharedManager] startSession];

  self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
  self.window.rootViewController = self.rootNavController;
  [self.window makeKeyAndVisible];
  return YES;
}
							
- (void)applicationWillResignActive:(UIApplication *)application
{
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
  // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
  // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
  // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
}

- (void)applicationWillTerminate:(UIApplication *)application
{
  // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

@end
